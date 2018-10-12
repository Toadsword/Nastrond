from SFGE import *
import math


def sign(x):
    return x and (1, -1)[x < 0]


class Pirate(Component):
    shooting_range: int
    is_dead: bool
    direction: Vector2f
    transform: Transform2d
    speed: float
    rot_speed: float

    def init(self):
        self.transform = self.get_component(Component.Transform2d)
        self.is_dead = False
        self.shooting_range = 200
        self.speed = 20
        self.rot_speed = 30
        self.direction = Vector2f.down

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
                wanted_direction1 = Vector2f(-wanted_direction.y, wanted_direction.x)
                wanted_direction2 = Vector2f(wanted_direction.y, -wanted_direction.x)
                if(math.fabs(Vector2f.angle_between(wanted_direction1, self.direction)) > math.fabs(Vector2f.angle_between(wanted_direction2, self.direction))):
                    wanted_direction = wanted_direction2
                else:
                    wanted_direction = wanted_direction1
            wanted_direction /= wanted_direction.magnitude
            self.direction.rotate(sign(Vector2f.angle_between(self.direction, wanted_direction))*dt*self.rot_speed)
            self.direction /= self.direction.magnitude
            #self.direction = wanted_direction
        self.transform.euler_angle = Vector2f.angle_between(self.direction, Vector2f.down)
        #shoot depending on timer

        self.transform.position += self.direction*dt*self.speed
