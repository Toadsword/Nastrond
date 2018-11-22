from SFGE import *


class PirateBullet(Component):
    speed = 200.0

    def init(self):
        self.range = Pirate.shooting_range
        self.timer = Timer(self.range/self.speed, self.range/self.speed)
        self.transform = self.get_component(Component.Transform2d)

    def update(self, dt):
        self.transform.position += self.direction*dt*self.speed
        self.timer.update(dt)

        if self.timer.is_over():
            self.pirate_manager.destroy_bullet(self)