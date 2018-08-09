from typing import Any

from SFGE import *


class PlayerCharacterBasic(Component):
    speed = None  # type: float
    body = None  # type: Body2d
    foot = None  # type: int

    def init(self):
        self.foot = 0
        self.body = self.get_component(Component.Body)
        self.speed = 1.0

    def update(self, dt):
        horizontal_move = 0.0
        vertical_move = 0.0

        if input_manager.keyboard.is_key_held(KeyboardManager.Key.Left):
            horizontal_move -= 1.0
        if input_manager.keyboard.is_key_held(KeyboardManager.Key.Right):
            horizontal_move += 1.0
        if input_manager.keyboard.is_key_held(KeyboardManager.Key.Up):
            vertical_move -= 1.0
        if input_manager.keyboard.is_key_held(KeyboardManager.Key.Down):
            vertical_move += 1.0

        if self.body:
            self.body.velocity = b2Vec2(horizontal_move, vertical_move)

