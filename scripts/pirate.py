from SFGE import *


class Pirate(Component):
    is_dead: bool
    direction: Vector2f
    transform: Transform2d
    pirate_manager: PirateManager

    def init(self):
        self.transform = self.get_component(Component.Transform2d)
        self.is_dead = False
        
    def update(self, dt):
        #TODO get closest none dead pirate boat not self
        closest_pirate = self.pirate_manager.get_closest_pirate(self)
        #TODO go to closest shooting position if not in range already

        #TODO rotate to be able to shoot

        #TODO shoot depending on timer

        pass
