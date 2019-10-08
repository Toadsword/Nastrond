def auto_import():
    from scripts import *


class Vector2f:
    """SFML and default SFGE Vector type when not working on the physic"""
    def __init__(self):
        self.x = 0.0
        self.y = 0.0
        self.magnitude = 0.0


class Vec2f:
    def __init__(self):
        self.x = 0.0
        self.y = 0.0
        self.magnitude = 0.0


class Vector2i:
    """SFML and default SFGE Vector type when not working on the physic"""
    def __init__(self):
        self.x = 0
        self.y = 0
        self.magnitude = 0.0


class b2Vec2:
    """Box2d Vector type used for physics related"""
    def __init__(self):
        self.x = 0.0
        self.y = 0.0
        self.magnitude = 0.0


class Sprite:
    def set_texture(self, texture):
        pass

    def load_texture(self, texture_name: str):
        pass

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


class System:
    def init(self):
        pass
    
    def update(self, dt):
        pass
    
    def destroy(self):
        pass

class ComponentManager():
    def add_component(self, entity):
        pass

    def destroy_component(self, entity):
        pass

    def get_component(self, entity):
        pass

class SpriteManager(System):
    def add_component(self, entity):
        return Sprite()


class TextureManager(System):
    pass    


class Graphics2dManager(System):
    def __init__(self):
        self.texture_manager = TextureManager()
        self.sprite_manager = SpriteManager()


class SceneManager(System):
    def load_scene(self, scene_name):
        pass


class Transform2dManager(System, ComponentManager):
    pass


class PythonEngine(System):
    def load_pycomponent(self, entity, script_path):
        pass


class EntityManager(System):

    def create_entity(self, wanted_entity):
        pass

    def destroy_entity(self, entity):
        pass

    def has_components(self, entity, component):
        pass

class NavigationGraphManager(System):
    def get_path_from_to(self, vec2f, vecf2):
        pass
    def test(self, int):
        pass

class Body2dManager(System, ComponentManager):
    pass

class Physics2dManager(System):
    body2d_manager = None # type: Body2dManager


class Engine:
    def __init__(self):
        pass


class Component:
    Sprite = 0
    Shape = 0
    Body = 0
    Sound = 0
    Transform2d = 0
    Button = 0
    RectTransform = 0

    def init(self):
        pass

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

    def get_component(self, type):
        pass


class Transform2d():
    """Mandatory Component attached to the GameObject containing all the geometric important data of the GameObject"""
    def __init__(self):
        self.position = Vector2f()
        self.scale = Vector2f()
        self.euler_angle = 0.0


class RectTransform:
    def __init__(self):
        pass

    def contains(self, pos_x, pos_y):
        pass


class Sound:
    def play(self):
        pass

    def stop(self):
        pass


class Body2d:
    def __init__(self):
        self.velocity = b2Vec2()
        self.magnitude = 0.0

    def apply_force(self, force:b2Vec2):
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


class MouseManager:
    class MouseButton:
        Left = 0
        Right = 0
        Middle = 0
        ExtraOne = 0
        ExtraTwo = 0

    def is_button_down(self, mbutton):
        pass

    def is_button_held(self, mbutton):
        pass

    def is_button_up(self, mbutton):
        pass

    def get_local_position(self):
        pass

    def get_world_position(self):
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
    mouse = MouseManager()


class Button:
    def __init__(self):
        pass


engine = Engine()
input_manager = InputManager()
scene_manager = SceneManager()
transform2d_manager = Transform2dManager()
entity_manager = EntityManager()
physics2d_manager = Physics2dManager()
body2d_manager = Body2dManager()
graphics2d_manager = Graphics2dManager()
python_engine = PythonEngine()
navigation_graph_manager = NavigationGraphManager()
