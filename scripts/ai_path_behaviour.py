from SFGE import *
import math

class AiPathBehaviour(Component):
    transform: Transform2d

    def init(self):
        self.transform = self.get_component(Component.Transform2d)
        i : int
        i = 0
        navigation_graph_manager.test(i)

