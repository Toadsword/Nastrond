from typing import List

from SFGE import *


class PirateManager(System):

    pirates: List[Pirate]
    piratesNmb = 300

    def init(self):
        self.pirates = [
            python_engine.load_pycomponent(self.entity, "scripts/pirate.py") for i in range(self.piratesNmb)
        ]
        for pirate in self.pirates:
            pirate.pirate_manager = self

    def update(self, dt):
        for pirate in self.pirates:
            pirate.update(dt)

    def get_closest_pirate(self, pirate):
        assert isinstance(pirate, Pirate)
        closest_pirate = None
        closest_distance = 10000
        for other_pirate in self.pirates:
            if pirate is not other_pirate:
                distance = (other_pirate.transform.position-pirate.transform.position).magnitude
                if distance < closest_distance:
                    closest_distance = distance
                    closest_pirate = other_pirate
        return closest_pirate