from SFGE import *


class CameraManager(Component):
    cam_component: Camera
    position: Vec2f
    valuetest: bool

    def init(self):
        self.cam_component = self.get_component(Component.Camera)
        self.position = self.cam_component.get_position()      
        
    def update(self,dt):
        if input_manager.keyboard.is_key_held(KeyboardManager.Key.Up):
           self.position += Vec2f(0, -100*dt)
           
        if input_manager.keyboard.is_key_held(KeyboardManager.Key.Down):
           self.position += Vec2f(0, 100*dt)
           
        if input_manager.keyboard.is_key_held(KeyboardManager.Key.Left):
           self.position += Vec2f( -100*dt,0)
           
        if input_manager.keyboard.is_key_held(KeyboardManager.Key.Right):
           self.position += Vec2f( 100*dt,0)

        self.cam_component.set_position(self.position)
        print(input_manager.mouse.get_world_position())