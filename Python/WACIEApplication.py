import kivy
kivy.require('2.0.0')
import serial.tools.list_ports
from kivy.clock import Clock
from kivy.properties import ObjectProperty, Clock
from kivy.app import App
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.gridlayout import GridLayout
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.label import Label
from kivy.uix.textinput import TextInput
from kivy.uix.button import Button
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy_garden.matplotlib import FigureCanvasKivyAgg
import matplotlib.pyplot as plt
from kivy.graphics import Color, Rectangle
from kivy.core.window import Window
from kivy.uix.dropdown import DropDown
import random
import array as arr
import time

x_val = []
y_val = []

class StartScreen(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.layout = FloatLayout()
        
        with self.canvas.before:
            Color(152/255, 146/255, 116/255, 1)  # Red color (RGBA)
            self.rect = Rectangle(size=Window.size, pos=self.pos)
        
        # Bind size changes to update the background rectangle
        self.bind(size=self.update_rect)

        # Button
        start_button = Button(text="Start",font_size=60, size_hint=(None, None), size=(200, 100), pos_hint={'center_x': 0.5, 'center_y': 0.5})
        start_button.bind(on_press=self.switch_to_input_screen)
        self.layout.add_widget(start_button)

        self.add_widget(self.layout)

    def update_rect(self, instance, value):
        self.rect.size = instance.size
        self.rect.pos = instance.pos

    def switch_to_input_screen(self, instance):
        app = App.get_running_app()
        app.sm.current = 'input'


class InputScreen(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.port_number = None
        self.requestID = None
        
        self.timeCounter = 0
        self.dropdown = DropDown()
        self.populate_dropdown()

        with self.canvas:
            Color(152/255, 146/255, 116/255, 1)  # Background color
            self.rect = Rectangle(size=Window.size, pos=self.pos)
            self.bind(size=self.update_rect)  # Bind size changes
            self.main_box = BoxLayout(orientation='vertical',padding=5)
            self.waveGrid = GridLayout(cols=2)
            self.waveGrid.size_hint = (1, 5)  # Example size
            self.waveBox1 = BoxLayout(orientation="vertical")
            self.waveBox2 = BoxLayout(orientation="vertical")
            
            fig1, ax = plt.subplots()
            self.waveform1_graph = FigureCanvasKivyAgg(figure=fig1)
            self.waveform1_graph.size_hint = (1, 5)  # Example size
            self.waveform1_get = Button(text="Get Waveform 1")
            self.waveform1_get.bind(on_press=self.fetch_graph1)
            self.waveBox1.add_widget(self.waveform1_graph)
            self.waveBox1.add_widget(self.waveform1_get)

            fig2, ax = plt.subplots()
            self.waveform2_graph = FigureCanvasKivyAgg(figure=fig2)
            self.waveform2_graph.size_hint = (1, 5)  # Example size
            self.waveform2_get = Button(text="Get Waveform 2")
            self.waveform2_get.bind(on_press=self.fetch_graph2)
            self.waveBox2.add_widget(self.waveform2_graph)
            self.waveBox2.add_widget(self.waveform2_get)

            self.waveGrid.add_widget(self.waveBox1)
            self.waveGrid.add_widget(self.waveBox2)
            
            self.velocity_label = Label(text='P-Wave Velocity: 00.00')
            self.concrete_label = Label(text='Concrete State: STRONG')
            self.estimate_label = Label(text='Estimated Lifespan: 25 years')

            self.dropdown_button = Button(text="Select Port", size_hint=(None, None), size=(500, 50),pos_hint={'center_x': 0.5, 'center_y': 0.5})
            self.dropdown_button.bind(on_release=self.dropdown.open)
            
            self.dropdown.bind(on_select=self.on_select)

            self.main_box.add_widget(self.dropdown_button)
            self.main_box.add_widget(self.waveGrid)
            self.main_box.add_widget(self.velocity_label)
            self.main_box.add_widget(self.concrete_label)
            self.main_box.add_widget(self.estimate_label)
            
            self.add_widget(self.main_box)

    def update_rect(self, instance, value):
        self.rect.size = instance.size
        self.rect.pos = instance.pos

    def set_background_color(self):
        with self.canvas.before:
            Color(152/255, 146/255, 116/255, 1)  # Background color
            self.rect = Rectangle(size=self.size, pos=self.pos)

    def work(self):
        Clock.schedule_interval(self.read_serial_data, 1 / 30)
        
    def rest(self):
        Clock.unschedule(self.read_serial_data)

    def fetch_graph1(self,instance):
        self.requestID = "Req1\n"
        self.timeCounter = 0
        global ser
        print("Clearing Graph")
        x_val.clear()
        y_val.clear()
        print(x_val,y_val)
        print(self.port_number)
        if self.port_number is None:
            print("Error: Port number is not selected.")
            return
        ser = serial.Serial(port=self.port_number, baudrate=115200)
        if(ser.isOpen()):
            self.waveform1_get.text = "Fetching Graph 1..."
            time.sleep(1)
            ser.flushInput()
            ser.flushOutput()
            print(self.requestID)
            ser.write(self.requestID.encode('utf-8'))
            time.sleep(1)
            self.work()


    def fetch_graph2(self,instance):
        self.requestID = "Req2\n"
        self.timeCounter = 0
        global ser
        print("Clearing Graph")
        x_val.clear()
        y_val.clear()
        print(x_val,y_val)
        print(self.port_number)
        if self.port_number is None:
            print("Error: Port number is not selected.")
            return
        ser = serial.Serial(port=self.port_number, baudrate=115200)
        if(ser.isOpen()):
            self.waveform2_get.text = "Fetching Graph 2..."
            time.sleep(1)
            ser.flushInput()
            ser.flushOutput()
            print(self.requestID)
            ser.write(self.requestID.encode('utf-8'))
            time.sleep(1)
            self.work()

    def read_serial_data(self, *args):
        
        self.timeCounter += 1
        if(self.timeCounter <= 151):
            
            serial_data = ser.readline().decode('utf-8')  # Read a line from serial and decode it
            print("Received data:", serial_data)
            serial_data = serial_data[0:-1]
            Data = serial_data.split()
            if(self.timeCounter <= 150):
                if(Data[0].isnumeric()):
                    x_val.append(int(Data[0]))
                    
                if(Data[1].isnumeric()):
                    y_val.append(int(Data[1]))
            else:
                Tap_time = float(Data[1])/1000
                print("Got Tap_time : " + str(Tap_time) + " sec")
            
        else:
            # print(x_val,y_val)
            if(self.requestID == "Req1\n"):
                self.plot_graph1()
                self.waveform1_get.text = "Get Waveform 1"
            elif(self.requestID == "Req2\n"):
                self.plot_graph2()
                self.waveform2_get.text = "Get Waveform 2"
            ser.close()
            self.rest()
            
    def plot_graph1(self):
        fig1, ax1 = plt.subplots()
        ax1.plot(x_val, y_val)
        ax1.set_xlabel('X')
        ax1.set_ylabel('Y')
        ax1.set_title('Wave Graph 1')

        self.waveBox1.remove_widget(self.waveform1_graph)
        self.waveBox1.remove_widget(self.waveform1_get)
        # Convert Matplotlib figure to Kivy-compatible canvas
        self.waveform1_graph = FigureCanvasKivyAgg(figure=fig1)
        self.waveform1_graph.size_hint = (1, 5)  # Example size
        self.waveBox1.add_widget(self.waveform1_graph)
        self.waveBox1.add_widget(self.waveform1_get)
        
    def plot_graph2(self):
        fig2, ax2 = plt.subplots()
        ax2.plot(x_val, y_val)
        ax2.set_xlabel('X')
        ax2.set_ylabel('Y')
        ax2.set_title('Wave Graph 2')

        self.waveBox2.remove_widget(self.waveform2_graph)
        self.waveBox2.remove_widget(self.waveform2_get)
        # Convert Matplotlib figure to Kivy-compatible canvas
        self.waveform2_graph = FigureCanvasKivyAgg(figure=fig2)
        self.waveform2_graph.size_hint = (1, 5)  # Example size
        self.waveBox2.add_widget(self.waveform2_graph)
        self.waveBox2.add_widget(self.waveform2_get)

    def populate_dropdown(self):
        com_ports = serial.tools.list_ports.comports()
        for port in com_ports:
            btn = Button(text=str(port), size_hint_y=None, height=44)
            btn.bind(on_release=lambda btn: self.dropdown.select(btn.text))
            self.dropdown.add_widget(btn)

    def on_select(self, instance, value):
        self.port_number = value.split()[0]
        self.dropdown_button.text = "{}".format(self.port_number)
        
class SampleGraphApp(App):
    def build(self):
        self.title = 'WACIE'

        self.sm = ScreenManager()

        self.start_screen = StartScreen(name='start')
        self.input_screen = InputScreen(name='input')
        self.input_screen.bind(on_pre_enter=lambda _: self.input_screen.set_background_color())  # Bind the background color setter

        self.sm.add_widget(self.start_screen)
        self.sm.add_widget(self.input_screen)

        return self.sm

if __name__ == '__main__':
    SampleGraphApp().run()