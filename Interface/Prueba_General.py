from threading import Thread
from tkinter import *
import tkinter.messagebox
import customtkinter
import serial
import time

pot = 0.0
ilu = 0.0
amax = 0.0
amin = 0.0
slivalues = 0
servo_control = 0

customtkinter.set_appearance_mode("Dark")  # Modes: "System" (standard), "Dark", "Light"
customtkinter.set_default_color_theme("green")  # Themes: "blue" (standard), "green", "dark-blue"


try:
    arduino = serial.Serial("COM4", 9600, timeout=1)
except:
    print("Error de coneccion con el puerto")


def DatosA():

    # arduino.reset_input_buffer()
    while (True):
        time.sleep(1)
        global servo_control, slivalues
        msg = str(servo_control)+","+str(slivalues)
        # print(msg)
        arduino.write((msg + '\n').encode())
        arduino.flush()
        global datos, pot, ilu, amax, amin
        datos = arduino.readline().decode()
        datos = datos.rstrip('\n')
   


thread = Thread(target=DatosA)
thread.start()



class App(customtkinter.CTk):
    
    

    def __init__(self):
        super().__init__()

        # configure window
        self.title("CustomTkinter complex_example.py")
        self.geometry(f"{1100}x{580}")

        # configure grid layout (4x4)
        self.grid_columnconfigure(1, weight=1)
        self.grid_columnconfigure((2, 3), weight=1)
        self.grid_rowconfigure((0, 1, 2), weight=1)

        # create sidebar frame with widgets
        self.sidebar_frame = customtkinter.CTkFrame(self, width=140, corner_radius=0)
        self.sidebar_frame.grid(row=0, column=0, rowspan=4, sticky="nsew")
        self.sidebar_frame.grid_rowconfigure(4, weight=1)
        self.logo_label = customtkinter.CTkLabel(self.sidebar_frame, text="Pulidora Automática", font=customtkinter.CTkFont(size=20, weight="bold"))
        self.logo_label.grid(row=0, column=0, padx=20, pady=(20, 10))

        self.tabview = customtkinter.CTkTabview(self, width=240)
        self.tabview.grid(row=0, column=1, padx=(20, 20), pady=(20, 0), sticky="nsew")
        self.tabview.add("Selección Parámetros")
        self.tabview.add("Verificación Muestra")
        self.tabview.add("Control de los ejes")
        self.tabview.tab("Selección Parámetros").grid_columnconfigure(0, weight=1)  # configure grid of individual tabs
        self.tabview.tab("Verificación Muestra").grid_columnconfigure(0, weight=1)
        self.tabview.tab("Control de los ejes").grid_columnconfigure(0, weight=1)

        self.label_radio_group = customtkinter.CTkLabel(self.tabview.tab("Selección Parámetros"), text="Seleccionar Material")
        self.label_radio_group.grid(row=0, column=0, columnspan=1, padx=10, pady=10, sticky="")
        self.material_menu = tkinter.IntVar(value=3)
        self.radio_button_1 = customtkinter.CTkRadioButton(self.tabview.tab("Selección Parámetros"),text="Acero", command=self.material1,variable=self.material_menu, value=0)
        self.radio_button_1.grid(row=1, column=0, pady=10, padx=20, sticky="n")
        self.radio_button_2 = customtkinter.CTkRadioButton(self.tabview.tab("Selección Parámetros"), text="Aluminio",command=self.material2,variable=self.material_menu, value=1)
        self.radio_button_2.grid(row=2, column=0, pady=10, padx=20, sticky="n")
        self.radio_button_3 = customtkinter.CTkRadioButton(self.tabview.tab("Selección Parámetros"), text="Cobre",command=self.material3,variable=self.material_menu, value=2)
        self.radio_button_3.grid(row=3, column=0, pady=10, padx=20, sticky="n")

        self.label_radio_group = customtkinter.CTkLabel(self.tabview.tab("Selección Parámetros"), text="Seleccionar Diametro")
        self.label_radio_group.grid(row=0, column=1, columnspan=1, padx=10, pady=10, sticky="")
        self.diametro_menu = tkinter.IntVar(value=3)
        self.radio_button_1 = customtkinter.CTkRadioButton(self.tabview.tab("Selección Parámetros"), text="12 mm",command=self.diametro1,variable=self.diametro_menu, value=0)
        self.radio_button_1.grid(row=1, column=1, pady=10, padx=20, sticky="n")
        self.radio_button_2 = customtkinter.CTkRadioButton(self.tabview.tab("Selección Parámetros"), text="15 mm",command=self.diametro2,variable=self.diametro_menu, value=1)
        self.radio_button_2.grid(row=2, column=1, pady=10, padx=20, sticky="n")
        self.radio_button_3 = customtkinter.CTkRadioButton(self.tabview.tab("Selección Parámetros"), text="20 mm",command=self.diametro3,variable=self.diametro_menu, value=2)
        self.radio_button_3.grid(row=3, column=1, pady=10, padx=20, sticky="n")

        self.check_proceso_1= tkinter.StringVar(value="off")
        self.check_proceso_2= tkinter.StringVar(value="off")
        self.check_proceso_3= tkinter.StringVar(value="off")
        self.check_proceso_4= tkinter.StringVar(value="off")

        self.label_radio_group = customtkinter.CTkLabel(self.tabview.tab("Selección Parámetros"), text="Seleccionar Procesos")
        self.label_radio_group.grid(row=0, column=2, columnspan=1, padx=10, pady=10, sticky="")
        self.checkbox_1 = customtkinter.CTkCheckBox(master=self.tabview.tab("Selección Parámetros"),text="Corte",command=self.checkbox_event,variable=self.check_proceso_1,onvalue="on",offvalue="off")
        self.checkbox_1.grid(row=1, column=2, pady=(20, 0), padx=20, sticky="n")
        self.checkbox_2 = customtkinter.CTkCheckBox(master=self.tabview.tab("Selección Parámetros"),text="Lijado",command=self.checkbox_event,variable=self.check_proceso_2,onvalue="on",offvalue="off")
        self.checkbox_2.grid(row=2, column=2, pady=(20, 0), padx=20, sticky="n")
        self.checkbox_3 = customtkinter.CTkCheckBox(master=self.tabview.tab("Selección Parámetros"),text="Pulido",command=self.checkbox_event,variable=self.check_proceso_3,onvalue="on",offvalue="off")
        self.checkbox_3.grid(row=3, column=2, pady=20, padx=20, sticky="n")
        self.checkbox_3 = customtkinter.CTkCheckBox(master=self.tabview.tab("Selección Parámetros"),text="Inspeccion",command=self.checkbox_event,variable=self.check_proceso_4,onvalue="on",offvalue="off")
        self.checkbox_3.grid(row=4, column=2, pady=20, padx=20, sticky="n")

        self.sidebar_button_1 = customtkinter.CTkButton(self.tabview.tab("Selección Parámetros"),text="Detener", command=self.boton1)
        self.sidebar_button_1.grid(row=5, column=2, padx=20, pady=10)

        self.sidebar_button_2 = customtkinter.CTkButton(self.tabview.tab("Selección Parámetros"),text="Iniciar", command=self.enviar_seleccion)
        self.sidebar_button_2.grid(row=5, column=3, padx=20, pady=10)

        self.textbox = customtkinter.CTkTextbox(self.tabview.tab("Selección Parámetros"), width=250)
        self.textbox.grid(row=7, column=1, padx=(20, 0), pady=(20, 0), sticky="nsew")


        self.textbox = customtkinter.CTkTextbox(self.tabview.tab("Control de los ejes"), width=200)
        self.textbox.grid(row=0, column=0, padx=(20, 0), pady=(20, 0), sticky="nsew")

        self.set_home = customtkinter.CTkButton(self.tabview.tab("Control de los ejes"),text="Set Home")
        self.set_home.grid(row=0, column=1, padx=20, pady=10)
            
        self.go_home = customtkinter.CTkButton(self.tabview.tab("Control de los ejes"),text="Go to Home")
        self.go_home.grid(row=0, column=2, padx=20, pady=10)

        self.ejex = customtkinter.CTkLabel(self.tabview.tab("Control de los ejes"), text="Eje X")
        self.ejex.grid(row=2, column=0, columnspan=1, padx=10, pady=10, sticky="")
        self.slider_x = customtkinter.CTkSlider(self.tabview.tab("Control de los ejes"))
        self.slider_x.grid(row=3, column=0, rowspan=5, padx=(10, 10), pady=(10, 10), sticky="ns")

        self.ejey = customtkinter.CTkLabel(self.tabview.tab("Control de los ejes"), text="Eje Y")
        self.ejey.grid(row=2, column=1, columnspan=1, padx=10, pady=10, sticky="")
        self.slider_y = customtkinter.CTkSlider(self.tabview.tab("Control de los ejes"),from_=0,to=50, command=self.sliderz_event)
        self.slider_y.grid(row=3, column=1, rowspan=5, padx=(10, 10), pady=(10, 10), sticky="ns")

        self.ejez = customtkinter.CTkLabel(self.tabview.tab("Control de los ejes"), text="Eje Z")
        self.ejez.grid(row=2, column=2, columnspan=1, padx=10, pady=10, sticky="")
        self.slider_z = customtkinter.CTkSlider(self.tabview.tab("Control de los ejes"))
        self.slider_z.grid(row=3, column=2, rowspan=5, padx=(10, 10), pady=(10, 10), sticky="ns")

        # self.enviar_pos = customtkinter.CTkButton(self.tabview.tab("Control de los ejes"),text="Enviar posicion")
        # self.enviar_pos.grid(row=1, column=1, padx=20, pady=10)


    def sliderz_event(value):
        print(value)

    def open_input_dialog_event(self):
        dialog = customtkinter.CTkInputDialog(text="Type in a number:", title="CTkInputDialog")
        print("CTkInputDialog:", dialog.get_input())

    def boton1(self):
        msg = "1"
        arduino.write((msg + '\n').encode())
        arduino.flush()

    def boton2(self):
        msg = "2"
        arduino.write((msg + '\n').encode())
        arduino.flush()     

    def material1(self):
        global material
        material= "1"
        #print(material)

    def material2(self):
        global material
        material= "2"
        #print(material)

    def material3(self):
        global material
        material= "3"
        #print(material)

    
    def diametro1(self):
        global diametro
        diametro= "1"
        #print(diametro)

    def diametro2(self):
        global diametro
        diametro= "2"
        #print(diametro)

    def diametro3(self):
        global diametro
        diametro= "3"
        #print(diametro)

    def sidebar_button_event(self):
        print("sidebar_button click")

    def enviar_seleccion(self):
        global material,diametro

        if material == "1":
            print("Acero")
            mate="1"
        elif material == "2":
            print("Aluminio")
            mate="2"
        elif material == "3":
            print("Cobre")
            mate="3"

        if diametro == "1":
            print("12mm")
            dia="1"
        elif diametro == "2":
            print("15mm")
            dia="2"
        elif diametro == "3":
            print("20mm")
            dia="3"

        if self.check_proceso_1.get() == "on":
            print("Corte")
            corte="1"
        else:
            corte="0"
        if self.check_proceso_2.get() == "on":
            print("Lijado")
            lijado="1"
        else:
            lijado="0"
        if self.check_proceso_3.get() == "on":
            print("Pulido")
            pulido="1"
        else:
            pulido="0"
        if self.check_proceso_4.get() == "on":
            print("Revision")
            inspec="1"
        else:
            inspec="0"

        selec=mate+","+dia+","+corte+","+lijado+","+pulido+","+inspec
        print(selec)
        arduino.write((selec + '\n').encode())
        arduino.flush()

    def checkbox_event (self):
        pass

if __name__ == "__main__":
    app = App()
    app.mainloop()

arduino.close()