from SFGE import *


class PlayerCharacterBasic(Component):

    def init(self):
        self.speed = 1.0
        self.body = self.game_object.get_component(Component.Body)
        self.foot = 0

    def update(self, dt):
        horizontal_move = 0.0
        jump_button = False
        if input_manager.keyboard.is_key_held(KeyboardManager.Key.Up):
            jump_button = True
        if input_manager.keyboard.is_key_held(KeyboardManager.Key.Left):
            horizontal_move -= 1.0
        if input_manager.keyboard.is_key_held(KeyboardManager.Key.Right):
            horizontal_move += 1
        #d *= self.speed
        if self.body:
            self.body.velocity = b2Vec2(horizontal_move, self.body.velocity.y)
            if self.foot > 0 and jump_button:
                self.body.add_force(b2Vec2(0,-100))

    def on_trigger_enter(self, collider):
        colliding_body = collider.game_object.get_component(Component.Body)
        if collider.is_trigger() and colliding_body.body_type == Body2d.STATIC_BODY:
            self.foot += 1

    def on_trigger_exit(self, collider):
        colliding_body = collider.game_object.get_component(Component.Body)
        if collider.is_trigger() and colliding_body.body_type == Body2d.STATIC_BODY:
            self.foot -= 1