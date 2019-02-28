from SFGE import *
import sys

from SFGE import Timer


class ComponentTest(Component):

    #timer: Timer

    def init(self):
        pass
#self.timer = Timer(1.0, 1.0)
#sprite_test = self.get_component(SpriteTest)
#if sprite_test:
#    print("Youpi "+str(sprite_test))
#else:
#    sys.stderr.write("No Sprite Test from Component Test\n")
#    sys.stderr.flush()

    def update(self, dt):
        pass
#print("Hello update from Python with dt: " + str(dt))
#self.timer.update(dt)
#if self.timer.is_over():
#    print("Timer over, reset")
#    self.timer.reset()

    def __del__(self):
        #sys.stderr.write("DELETING COMPONENT TEST PYTHON\n")
        sys.stderr.flush()
