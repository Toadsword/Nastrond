print("Python SFGE")


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
        self.modules = []


class GameObject:
    """Typical GameObject of the Scene, contains a list of Components that can get using the different getter functions"""
    def __init__(self):
        self.name = ""
        self.transform = Transform()

    def get_component(self, componentType):
        """Get the first component of type given as arg"""
        pass

    def get_component(self, pycomponentType):
        """Get the first component of type given as arg"""
        pass

    def get_components(self, componentType):
        """Get the list of components of type given as arg"""
        pass


class Component:
    Sprite = 0
    Shape = 0
    Body = 0

    def __init__(self, game_object: GameObject, name):
        self.game_object = game_object
        self.name = name

    def update(self, dt):
        pass


class Transform(Component):
    """Mandatory Component attached to the GameObject containing all the geometric important data of the GameObject"""
    def __init__(self, game_object):
        pass

    def get_euler_angle(self):
        pass

    def set_euler_angle(self, euler_angle: float):
       pass

    def get_position(self):
        pass

    def set_position(self, position: Vector2f):
        pass

    def get_scale(self):
        pass

    def set_scale(self, scale: Vector2f):
        pass


class Collider(Component):

    def is_trigger(self):
        return False


class Body2d(Component):
    STATIC_BODY = 0
    KINEMATIC_BODY = 0
    DYNAMIC_BODY = 0

    def __init__(self):
        self.mass = 0.0
        self.velocity = b2Vec2()
        self.body_type = 0

    def add_force(self, f):
        pass


class Shape(Component):
    def set_fill_color(self, color):
        pass


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