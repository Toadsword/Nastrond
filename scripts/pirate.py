from SFGE import *
import math


def sign(x):
    return x and (1, -1)[x < 0]


class Pirate(Component):
    shooting_range: int
    is_dead: bool
    direction: Vector2f
    transform: Transform2d
    speed = 20
    rot_speed = 20
    shooting_range = 200

    def init(self):
        self.transform = self.get_component(Component.Transform2d)
        self.is_dead = False
        self.direction = Vector2f.down
        self.shooting_timer = Timer(-1.0, 2.0)

    def fixed_update(self, dt):
        #get closest none dead pirate boat not self
        closest_pirate = self.pirate_manager.get_closest_pirate(self)
        if closest_pirate is not None:
            #go to closest shooting position if not in range already
            shooting_pos = closest_pirate.transform.position
            delta_pos = shooting_pos-self.transform.position

            if delta_pos.magnitude > self.shooting_range:
                wanted_direction = delta_pos/delta_pos.magnitude
            else:
                # rotate to be able to shoot
                wanted_direction = delta_pos/delta_pos.magnitude
                wanted_direction = Vector2f(-wanted_direction.y, wanted_direction.x)
                if self.shooting_timer.is_over():
                    self.pirate_manager.fire_bullet(self, closest_pirate)
                    self.shooting_timer.reset()
                else:
                    self.shooting_timer.update(dt)
            wanted_direction /= wanted_direction.magnitude
            self.direction = Vector2f.lerp(self.direction, wanted_direction, dt)
            #self.direction.rotate(Vector2f.angle_between(self.direction, wanted_direction) * dt )#* self.rot_speed)
            self.direction /= self.direction.magnitude
            #self.direction = wanted_direction

        self.transform.euler_angle = Vector2f.angle_between(self.direction, Vector2f.down)
        #shoot depending on timer

        self.transform.position += self.direction*dt*self.speed
