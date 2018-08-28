from SFGE import *
import sys


class SpriteTest(Component):
    transform: Transform2d
    speed: float
    direction: Vector2f

    def init(self):
        self.speed = 4.0
        self.direction = Vector2f(2.0, 4.0)
        self.transform = self.get_component(Component.Transform)
        sprite_test = self.get_component(ComponentTest)
        if sprite_test:
            print("Youpi " + str(sprite_test))
        else:
            sys.stderr.write("No ComponentTest from Sprite Test\n")
            sys.stderr.flush()
        
    def update(self, dt):
        print("Update from python sprite_test: "+str(dt))
        current_pos = self.transform.position

        delta_pos = self.direction*self.speed*dt
        self.transform.position = current_pos + delta_pos
    
    def __del__(self):
        sys.stderr.write("DELETING SPRITE TEST PYTHON\n")
        sys.stderr.flush()