from threading import Thread
import tkinter
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
datosfinales = ""

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
        print(datos)



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

        self.label_radio_group = customtkinter.CTkLabel(self.tabview.tab("Selección Parámetros"), text="Seleccionar Material")
        self.label_radio_group.grid(row=0, column=0, columnspan=1, padx=10, pady=10, sticky="")
        self.material_menu = tkinter.IntVar(value=0)
        self.radio_button_1 = customtkinter.CTkRadioButton(self.tabview.tab("Selección Parámetros"), variable=self.material_menu, value=0)
        self.radio_button_1.grid(row=1, column=0, pady=10, padx=20, sticky="n")
        self.radio_button_2 = customtkinter.CTkRadioButton(self.tabview.tab("Selección Parámetros"), variable=self.material_menu, value=1)
        self.radio_button_2.grid(row=2, column=0, pady=10, padx=20, sticky="n")
        self.radio_button_3 = customtkinter.CTkRadioButton(self.tabview.tab("Selección Parámetros"), variable=self.material_menu, value=2)
        self.radio_button_3.grid(row=3, column=0, pady=10, padx=20, sticky="n")

        self.label_radio_group = customtkinter.CTkLabel(self.tabview.tab("Selección Parámetros"), text="Seleccionar Diametro")
        self.label_radio_group.grid(row=0, column=1, columnspan=1, padx=10, pady=10, sticky="")
        self.diametro_menu = tkinter.IntVar(value=0)
        self.radio_button_1 = customtkinter.CTkRadioButton(self.tabview.tab("Selección Parámetros"), command=self.sidebar_button_event,variable=self.diametro_menu, value=0)
        self.radio_button_1.grid(row=1, column=1, pady=10, padx=20, sticky="n")
        self.radio_button_2 = customtkinter.CTkRadioButton(self.tabview.tab("Selección Parámetros"), variable=self.diametro_menu, value=1)
        self.radio_button_2.grid(row=2, column=1, pady=10, padx=20, sticky="n")
        self.radio_button_3 = customtkinter.CTkRadioButton(self.tabview.tab("Selección Parámetros"), variable=self.diametro_menu, value=2)
        self.radio_button_3.grid(row=3, column=1, pady=10, padx=20, sticky="n")

        self.label_radio_group = customtkinter.CTkLabel(self.tabview.tab("Selección Parámetros"), text="Seleccionar Procesos")
        self.label_radio_group.grid(row=0, column=2, columnspan=1, padx=10, pady=10, sticky="")
        self.checkbox_1 = customtkinter.CTkCheckBox(master=self.tabview.tab("Selección Parámetros"))
        self.checkbox_1.grid(row=1, column=2, pady=(20, 0), padx=20, sticky="n")
        self.checkbox_2 = customtkinter.CTkCheckBox(master=self.tabview.tab("Selección Parámetros"))
        self.checkbox_2.grid(row=2, column=2, pady=(20, 0), padx=20, sticky="n")
        self.checkbox_3 = customtkinter.CTkCheckBox(master=self.tabview.tab("Selección Parámetros"))
        self.checkbox_3.grid(row=3, column=2, pady=20, padx=20, sticky="n")

        self.sidebar_button_1 = customtkinter.CTkButton(self.tabview.tab("Selección Parámetros"), command=self.sidebar_button_event)
        self.sidebar_button_1.grid(row=5, column=2, padx=20, pady=10)

        self.textbox = customtkinter.CTkTextbox(self, width=250)
        self.textbox.grid(row=6, column=1, padx=(20, 0), pady=(20, 0), sticky="nsew")

    def open_input_dialog_event(self):
        dialog = customtkinter.CTkInputDialog(text="Type in a number:", title="CTkInputDialog")
        print("CTkInputDialog:", dialog.get_input())

    def sidebar_button_event(self):
        print("sidebar_button click")


if __name__ == "__main__":
    app = App()
    app.mainloop()

arduino.close()