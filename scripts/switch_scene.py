from SFGE import *

class SwitchScene(Component):
    def init(self):
        pass

    def update(self, dt):
        if input_manager.keyboard.is_key_down(KeyboardManager.Key.Space):
            scene_manager.load_scene("SceneTest")