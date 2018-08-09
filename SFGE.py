


class Vector2f:
    """SFML and default SFGE Vector type when not working on the physic"""
    def __init__(self):
        self.x = 0
        self.y = 0


class b2Vec2:
    """Box2d Vector type used for physics related"""
    def __init__(self):
        self.x = 0.0
        self.y = 0.0


class Timer:
    """Timer used for update loop and """
    def __init__(self, time, period):
        self.time = 0
        self.period = 0
    
    def reset(self):
        """Set the value of time to period"""
        pass
    
    def update(self, dt):
        """Update the timer of the dt given as parameter"""
        pass
    
    def get_current(self):
        """Get the current ratio of time between 0.0 and 1.0"""
        pass
    
    def get_current_time(self):
        """Get the current time from the start """
        pass
    
    def is_over(self):
        """Return the status of the termination of the timer"""
        pass


class Module:
    def init(self):
        pass
    
    def update(self, dt):
        pass
    
    def destroy(self):
        pass


class SpriteManager:
    pass


class TextureManager:
    pass    


class GraphicsManager(Module):
    pass


class SceneManager(Module):
    def load_scene(self, scene_name):
        pass


class Engine:
    def __init__(self):
        pass


class Component:
    Sprite = 0
    Shape = 0
    Body = 0
    Sound = 0

    def update(self, dt):
        pass

    def fixed_update(self, dt):
        pass

    def on_trigger_enter(self, collider):
        pass

    def on_trigger_exit(self, collider):
        pass

    def on_collision_enter(self, collider):
        pass

    def on_collision_exit(self, collider):
        pass


class Transform():
    """Mandatory Component attached to the GameObject containing all the geometric important data of the GameObject"""
    def __init__(self):
        self.position = Vector2f()
        self.scale = Vector2f()
        self.angle = 0.0

class Body2d:
    def __init__(self):
        self.velocity = b2Vec2()



class KeyboardManager:
    class Key:
        Space = 0
        Up = 0
        Down = 0
        Left = 0
        Right = 0
    def is_key_held(self, key):
        pass

    def is_key_down(self, key):
        pass

    def is_key_up(self, key):
        pass


class Color:
    Red = 0
    Green = 0
    Blue = 0
    Yellow = 0
    Black = 0
    White = 0
    Magenta = 0
    Cyan = 0
    Transparent = 0


class InputManager:
    keyboard = KeyboardManager()


engine = Engine()
input_manager = InputManager()
scene_manager = SceneManager()
