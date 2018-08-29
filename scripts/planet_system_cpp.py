
from SFGE import *


class PlanetSystemCpp(System):

    def init(self):
        self.planet_system = PlanetSystem(engine)
        self.planet_system.init()

    def update(self, dt):
        self.planet_system.fixed_update(self, dt)

    def fixed_update(self):
        self.planet_system.fixed_update(self)
