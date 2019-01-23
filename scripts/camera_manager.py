from SFGE import *


class CameraManager(Component):
    cam_component: Camera
    velocity: float
    detection_size_percent: float
    size_display: Vec2f
    position_display: Vec2f
    position: Vec2f
    mouse_position : Vector2f
    height_tilebar: int
    
    dir : Vec2f 
    result: Vector2f

    def init(self):
        self.cam_component = self.get_component(Component.Camera)
        self.position = self.cam_component.get_position()
        self.size_display = graphics2d_manager.get_size_window()
        self.velocity = 200
        self.detection_size_percent = 15;
        self.height_tilebar = 29

    def update(self,dt):
        #Keyboard
        dir = Vec2f(0.0, 0.0)
        if input_manager.keyboard.is_key_held(KeyboardManager.Key.Up):
           dir.y = 0.5
             
        if input_manager.keyboard.is_key_held(KeyboardManager.Key.Down):
           dir.y = -0.5
           
        if input_manager.keyboard.is_key_held(KeyboardManager.Key.Left):
           dir.x = 0.5
           
        if input_manager.keyboard.is_key_held(KeyboardManager.Key.Right):
           dir.x = -0.5
        #Mouse  
        self.position_display = graphics2d_manager.get_position_window()
        self.mouse_position  = input_manager.mouse.get_world_position()
        self.size_display = graphics2d_manager.get_size_window()
        print(self.mouse_position.y)
        
        if (self.mouse_position.x <= self.position_display.x or self.position_display.x + self.size_display.x <= self.mouse_position.x) or (self.mouse_position.y <= self.position_display.y + self.height_tilebar or self.position_display.y + self.size_display.y + self.height_tilebar  <= self.mouse_position.y):
            pass
        else:
            if self.mouse_position.y - self.position_display.y < self.size_display.y / 100 * self.detection_size_percent:
                result = Vector2f.lerp(Vector2f(0, self.mouse_position.y - self.position_display.y), Vector2f(0.0,0.0), 0.5)
                dir.y = -1 + (result.y) / (self.size_display.y / 100 * self.detection_size_percent)

            if self.mouse_position.y - self.position_display.y > self.size_display.y / 100 * (100-self.detection_size_percent):
                result = Vector2f.lerp(Vector2f(0, self.mouse_position.y - self.position_display.y), self.size_display, 0.5)
                dir.y = 1 + (result.y - self.size_display.y) / (self.size_display.y / 100*self.detection_size_percent)

            if self.mouse_position.x - self.position_display.x < self.size_display.x / 100 * self.detection_size_percent:
                result = Vector2f.lerp(Vector2f(self.mouse_position.x - self.position_display.x, 0), Vector2f(0.0,0.0), 0.5)
                dir.x = -1 + 2*((result.x) / (self.size_display.x / 100 * self.detection_size_percent))

            if self.mouse_position.x - self.position_display.x > self.size_display.x / 100 * (100-self.detection_size_percent):
                result = Vector2f.lerp(Vector2f(self.mouse_position.x - self.position_display.x, 0), self.size_display, 0.5)
                dir.x = 1 + 2*((result.x - self.size_display.x) / (self.size_display.x / 100 * self.detection_size_percent))
        self.position += dir * self.velocity * dt
        self.cam_component.set_position(self.position)