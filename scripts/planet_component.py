from SFGE import *
import math


class PlanetComponent(Component):
    gravity_const = 1000.0
    planet_mass: float
    center_mass: float
    screen_size: Vec2f
    body: Body2d
    transform: Transform2d

    def init(self):
        self.center_mass = 1000.0
        self.planet_mass = 1.0
        self.body = self.get_component(Component.Body)
        self.transform = self.get_component(Component.Transform2d)
        self.screen_size = engine.config.screen_size

        init_velocity = self.calculate_init_speed()
        self.body.velocity = init_velocity

        #force = self.calculate_new_force()
        #self.body.apply_force(force)

    def calculate_init_speed(self):
        delta_to_center = self.screen_size / 2.0 - self.transform.position
        vel_dir = Vector2f(-delta_to_center.y, delta_to_center.x)
        #perpendicular direction
        vel_dir = vel_dir / vel_dir.magnitude
        #normalizing
        speed = math.sqrt(self.calculate_new_force().magnitude / self.planet_mass * Physics2dManager.pixel2meter(delta_to_center.magnitude))
        #a = v^2 / r <=> v = sqrt(F / m * r)
        return b2Vec2(vel_dir.x * speed, vel_dir.y * speed)

    def calculate_new_force(self):
        delta_to_center = self.screen_size / 2.0 - self.transform.position
        r = delta_to_center.magnitude
        force = self.gravity_const * self.center_mass * self.planet_mass / (r*r)
        return Physics2dManager.pixel2meter(delta_to_center / delta_to_center.magnitude * force)

    def fixed_update(self, dt):
        force = self.calculate_new_force()
        self.body.apply_force(force)

        #print("Fixed Update Physics: "+str(force)+" velocity: "+str(self.body.velocity))
