from typing import List
import random
from SFGE import *


class PirateData:
    speed = 20
    rot_speed = 20
    shooting_range = 200

    def __init__(self):
        self.direction = Vec2f.down
        self.shooting_timer = Timer(-1.0, 2.0)
        self.entity = 0
        self.transform = None


class BulletData:
    speed = 200

    def __init__(self):
        self.range = PirateData.shooting_range
        self.timer = Timer(self.range/self.speed, self.range/self.speed)
        self.transform = None


class PirateManager(System):
    piratesNmb = 100

    def __init__(self, engine):
        self.screen_size = engine.config.screen_size
        size_x = self.screen_size.x
        size_y = self.screen_size.y
        print("Hello from Pirate Manager")
        System.__init__(self, engine)
        self.pirates = []
        self.bullets = []
        entity_manager.resize(self.piratesNmb*2)
        random.seed = 0
        for i in range(self.piratesNmb):
            print("Creating pirate "+str(i))
            new_entity = entity_manager.create_entity(i+1)
            transform = transform2d_manager.add_component(new_entity)
            transform.position = Vec2f(random.randint(0, size_x),
                                          random.randint(0, size_y))
            sprite = graphics2d_manager.sprite_manager.add_component(new_entity)
            texture = graphics2d_manager.texture_manager.load_texture("data/pirates/Ships/ship (1).png")
            sprite.set_texture(texture)
            pirate_data = PirateData()
            pirate_data.transform = transform
            pirate_data.entity = new_entity
            #self.pirates.append(python_engine.load_pycomponent(new_entity, "scripts/pirate.py"))
            self.pirates.append(pirate_data)

    def update(self, dt):
        for pirate in self.pirates:
            closest_pirate = self.get_closest_pirate(pirate)
            if closest_pirate != None:
                #go to closest shooting position if not in range already
                shooting_pos = closest_pirate.transform.position
                delta_pos = shooting_pos-pirate.transform.position

                if delta_pos.magnitude > PirateData.shooting_range:
                    wanted_direction = delta_pos/delta_pos.magnitude
                else:
                    # rotate to be able to shoot
                    wanted_direction = delta_pos/delta_pos.magnitude
                    wanted_direction = Vec2f(-wanted_direction.y, wanted_direction.x)
                    if pirate.shooting_timer.is_over():
                        self.fire_bullet(pirate, closest_pirate)
                        pirate.shooting_timer.period = 2.0+random.random()-0.5
                        pirate.shooting_timer.reset()
                    else:
                        pirate.shooting_timer.update(dt)
                wanted_direction /= wanted_direction.magnitude
                pirate.direction = Vec2f.lerp(pirate.direction, wanted_direction, dt)
                #self.direction.rotate(Vector2f.angle_between(self.direction, wanted_direction) * dt )#* self.rot_speed)
                pirate.direction /= pirate.direction.magnitude
                #self.direction = wanted_direction

            pirate.transform.euler_angle = Vec2f.angle_between(pirate.direction, Vec2f.down)
            #shoot depending on timer

            pirate.transform.position += pirate.direction*dt*pirate.speed

        for bullet in self.bullets:
            bullet.transform.position += bullet.direction*dt*BulletData.speed
            bullet.timer.update(dt)

            if bullet.timer.is_over():
                self.destroy_bullet(bullet)

    def fire_bullet(self, from_pirate, to_pirate):
        new_entity = entity_manager.create_entity(0)
        if new_entity != 0:
            #print("Shoot bullet "+str(new_entity))
            transform = transform2d_manager.add_component(new_entity)
            transform.position = from_pirate.transform.position
            sprite = graphics2d_manager.sprite_manager.add_component(new_entity)
            texture = graphics2d_manager.texture_manager.load_texture("data/pirates/Effects/cannonBall.png")
            sprite.set_texture(texture)
            #bullet = python_engine.load_pycomponent(new_entity, "scripts/pirate_bullet.py")
            #bullet.pirate_manager = self
            bullet = BulletData()
            bullet.entity = new_entity
            bullet.direction = to_pirate.transform.position-from_pirate.transform.position
            bullet.direction /= bullet.direction.magnitude
            bullet.transform = transform
            self.bullets.append(bullet)
            #bullet.init()

    def destroy_bullet(self, bullet):
        self.bullets.remove(bullet)
        entity_manager.destroy_entity(bullet.entity)

    def get_closest_pirate(self, pirate):
        closest_pirate = None
        closest_distance = 10000
        """
        for other_pirate in self.pirates:
            if pirate != other_pirate:
                distance = (other_pirate.transform.position-pirate.transform.position).magnitude
                if distance < closest_distance:
                    closest_distance = distance
                    closest_pirate = other_pirate
        return closest_pirate
        """
        for other_pirate in self.pirates:
            if pirate != other_pirate:
                distance = (transform2d_manager.get_component(other_pirate.entity).position-transform2d_manager.get_component(pirate.entity).position).magnitude
                if distance < closest_distance:
                    closest_distance = distance
                    closest_pirate = other_pirate
        return closest_pirate
