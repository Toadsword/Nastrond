from SFGE import *


# This script is used as a modal for every button
# the only thing needed to do here is to define what the action method do


class ButtonTest(Component):
    mousePosition: Vector2i

    def init(self):
        self.Button = self.get_component(Component.Button)
        self.RectTransform = self.get_component(Component.RectTransform)

    def update(self, dt):
        if input_manager.mouse.is_button_up(input_manager.mouse.MouseButton.Left):
            self.mousePosition = input_manager.mouse.get_local_position()
            print("User click detected at " + self.mousePosition.x.__str__() + " | " + self.mousePosition.y.__str__())
            if self.RectTransform.contains(self.mousePosition.x, self.mousePosition.y):
                print("User click in button")
                self.action()

    def action(self):
        print("Action launched")
