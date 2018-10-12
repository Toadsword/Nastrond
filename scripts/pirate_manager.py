from typing import List
import random
from SFGE import *


class PirateManager(System):

    piratesNmb = 10

    def __init__(self, engine):
        self.screen_size = engine.config.screen_size
        print("Hello from Pirate Manager")
        System.__init__(self, engine)
        self.pirates = []
        entity_manager.resize(self.piratesNmb)
        random.seed = 0
        for i in range(self.piratesNmb):
            print("Creating pirate "+str(i))
            new_entity = entity_manager.create_entity(i+1)
            transform = transform2d_manager.add_component(new_entity)
            transform.position = Vector2f(random.randint(0, self.screen_size.x),
                                          random.randint(0, self.screen_size.y))
            sprite = graphics2d_manager.sprite_manager.add_component(new_entity)
            texture = graphics2d_manager.texture_manager.load_texture("data/pirates/Ships/ship (1).png")
            sprite.set_texture(texture)
            self.pirates.append(python_engine.load_pycomponent(new_entity, "scripts/pirate.py"))

        for pirate in self.pirates:
            pirate.pirate_manager = self

    def get_closest_pirate(self, pirate):
        closest_pirate = None
        closest_distance = 10000
        for other_pirate in self.pirates:
            if pirate is not other_pirate:
                distance = (other_pirate.transform.position-pirate.transform.position).magnitude
                if distance < closest_distance:
                    closest_distance = distance
                    closest_pirate = other_pirate
        return closest_pirate
