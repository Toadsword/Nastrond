from SFGE import *


class PlaySound(Component):
    snd_component: Sound

    def init(self):
        self.snd_component = self.get_component(Component.Sound)

    def update(self, dt):
        if input_manager.keyboard.is_key_down(KeyboardManager.Key.Space):
            self.snd_component.play()
