import random
from SFGE import *


class PlanetSystem(System):

    entity_nmb = None  # type: int

    def init(self):
        self.entity_nmb = 100
        entity_manager.resize(self.entity_nmb)
        for i in range(self.entity_nmb):
            entity = entity_manager.create_entity()

            transform2d_manager.create_component(new_entity)
            transform = transform2d_manager.get_component(new_entity) # type: Transform2d
            transform.position = Vector2f(random.randint(0, 800), random.random(0, 800))



    def update(self, dt):
        pass
