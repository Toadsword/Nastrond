from SFGE import *
import random

body2d_manager = physics2d_manager.body2d_manager
sprite_manager = graphics2d_manager.sprite_manager
texture_manager = graphics2d_manager.texture_manager


class SwitchSceneSystem(System):

    screen_size: Vec2f

    def init(self):
        print("Init from SwitchSystem")
        self.entity_nmb = 100000
        self.screen_size = engine.config.screen_size
        entity_manager.resize(self.entity_nmb)
        for i in range(1, self.entity_nmb):
            new_entity = entity_manager.create_entity(i)
            if new_entity != 0:
                transform = transform2d_manager.add_component(new_entity)
                transform.position = Vec2f(random.randint(0, self.screen_size.x),
                                           random.randint(0, self.screen_size.y))

                body2d = body2d_manager.add_component(new_entity)  # type: Body2d

                sprite_manager.create_component(new_entity, "data/sprites/round.png")

    def update(self, dt):
        if input_manager.keyboard.is_key_down(KeyboardManager.Key.Space):
            scene_manager.load_scene("SceneManyEntities")
