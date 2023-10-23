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
valx = 0
valy = 0
valz = 0

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

 
# def sliderx_event(valuex):
#     global valx
#     valx=valuex
#     #print(valx)

# def slidery_event(valuey):
#     global valy
#     valy=valuey
#     #print(valy)

# def sliderz_event(valuez):
#     global valz
#     valz=valuez
#     #print(valz)

# def enviar_pos():
#     print(valx)
#     print(valy)
#     print(valz)

def incremento_x():
    global valx
    valx=valx+5;
    print(valx)

def decremento_x():
    global valx
    valx=valx-5;
    print(valx)


def incremento_y():
    global valy
    valy=valy+5;
    print(valy)

def decremento_y():
    global valy
    valy=valy-5;
    print(valy)

def incremento_z():
    global valz
    valz=valz+5;
    print(valz)

def decremento_z():
    global valz
    valz=valz-5;
    print(valz)

def reset_ejes():
    global valx,valy,valz
    valx=0
    valy=0
    valz=0

def boton1():
    msg = "1"
    arduino.write((msg + '\n').encode())
    arduino.flush()

def boton2():
    msg = "2"
    arduino.write((msg + '\n').encode())
    arduino.flush()     

def material1():
    global material
    material= "1"
    #print(material)

def material2():
    global material
    material= "2"
    #print(material)

def material3():
    global material
    material= "3"
    #print(material)


def diametro1():
    global diametro
    diametro= "1"
    #print(diametro)

def diametro2():
    global diametro
    diametro= "2"
    #print(diametro)

def diametro3():
    global diametro
    diametro= "3"
    #print(diametro)

def sidebar_button_event():
    print("sidebar_button click")

def enviar_seleccion():
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

    if check_proceso_1.get() == "on":
        print("Corte")
        corte="1"
    else:
        corte="0"
    if check_proceso_2.get() == "on":
        print("Lijado")
        lijado="1"
    else:
        lijado="0"
    if check_proceso_3.get() == "on":
        print("Pulido")
        pulido="1"
    else:
        pulido="0"
    if check_proceso_4.get() == "on":
        print("Revision")
        inspec="1"
    else:
        inspec="0"

    selec=mate+","+dia+","+corte+","+lijado+","+pulido+","+inspec
    print(selec)
    arduino.write((selec + '\n').encode())
    arduino.flush()


def checkbox_event ():
    pass
thread = Thread(target=DatosA)
thread.start()

app = customtkinter.CTk()

app.title("CustomTkinter complex_example.py")
app.geometry(f"{1100}x{580}")

tabview = customtkinter.CTkTabview(master=app, width=240)
tabview.grid(row=0, column=1, padx=(20, 20), pady=(20, 0), sticky="nsew")
tabview.add("Selección Parámetros")
tabview.add("Verificación Muestra")
tabview.add("Control de los ejes")
tabview.tab("Selección Parámetros").grid_columnconfigure(0, weight=1)  # configure grid of individual tabs
tabview.tab("Verificación Muestra").grid_columnconfigure(0, weight=1)
tabview.tab("Control de los ejes").grid_columnconfigure(0, weight=1)


label_radio_group = customtkinter.CTkLabel(tabview.tab("Selección Parámetros"), text="Seleccionar Material")
label_radio_group.grid(row=0, column=0, columnspan=1, padx=10, pady=10, sticky="")
material_menu = tkinter.IntVar(value=3)
radio_button_1 = customtkinter.CTkRadioButton(tabview.tab("Selección Parámetros"),text="Acero", command=material1,variable=material_menu, value=0)
radio_button_1.grid(row=1, column=0, pady=10, padx=20, sticky="n")
radio_button_2 = customtkinter.CTkRadioButton(tabview.tab("Selección Parámetros"), text="Aluminio",command=material2,variable=material_menu, value=1)
radio_button_2.grid(row=2, column=0, pady=10, padx=20, sticky="n")
radio_button_3 = customtkinter.CTkRadioButton(tabview.tab("Selección Parámetros"), text="Cobre",command=material3,variable=material_menu, value=2)
radio_button_3.grid(row=3, column=0, pady=10, padx=20, sticky="n")

label_radio_group = customtkinter.CTkLabel(tabview.tab("Selección Parámetros"), text="Seleccionar Diametro")
label_radio_group.grid(row=0, column=1, columnspan=1, padx=10, pady=10, sticky="")
diametro_menu = tkinter.IntVar(value=3)
radio_button_1 = customtkinter.CTkRadioButton(tabview.tab("Selección Parámetros"), text="12 mm",command=diametro1,variable=diametro_menu, value=0)
radio_button_1.grid(row=1, column=1, pady=10, padx=20, sticky="n")
radio_button_2 = customtkinter.CTkRadioButton(tabview.tab("Selección Parámetros"), text="15 mm",command=diametro2,variable=diametro_menu, value=1)
radio_button_2.grid(row=2, column=1, pady=10, padx=20, sticky="n")
radio_button_3 = customtkinter.CTkRadioButton(tabview.tab("Selección Parámetros"), text="20 mm",command=diametro3,variable=diametro_menu, value=2)
radio_button_3.grid(row=3, column=1, pady=10, padx=20, sticky="n")

check_proceso_1= tkinter.StringVar(value="off")
check_proceso_2= tkinter.StringVar(value="off")
check_proceso_3= tkinter.StringVar(value="off")
check_proceso_4= tkinter.StringVar(value="off")

label_radio_group = customtkinter.CTkLabel(tabview.tab("Selección Parámetros"), text="Seleccionar Procesos")
label_radio_group.grid(row=0, column=2, columnspan=1, padx=10, pady=10, sticky="")
checkbox_1 = customtkinter.CTkCheckBox(master=tabview.tab("Selección Parámetros"),text="Corte",command=checkbox_event,variable=check_proceso_1,onvalue="on",offvalue="off")
checkbox_1.grid(row=1, column=2, pady=(20, 0), padx=20, sticky="n")
checkbox_2 = customtkinter.CTkCheckBox(master=tabview.tab("Selección Parámetros"),text="Lijado",command=checkbox_event,variable=check_proceso_2,onvalue="on",offvalue="off")
checkbox_2.grid(row=2, column=2, pady=(20, 0), padx=20, sticky="n")
checkbox_3 = customtkinter.CTkCheckBox(master=tabview.tab("Selección Parámetros"),text="Pulido",command=checkbox_event,variable=check_proceso_3,onvalue="on",offvalue="off")
checkbox_3.grid(row=3, column=2, pady=20, padx=20, sticky="n")
checkbox_3 = customtkinter.CTkCheckBox(master=tabview.tab("Selección Parámetros"),text="Inspeccion",command=checkbox_event,variable=check_proceso_4,onvalue="on",offvalue="off")
checkbox_3.grid(row=4, column=2, pady=20, padx=20, sticky="n")

sidebar_button_1 = customtkinter.CTkButton(tabview.tab("Selección Parámetros"),text="Detener", command=boton1)
sidebar_button_1.grid(row=5, column=2, padx=20, pady=10)

sidebar_button_2 = customtkinter.CTkButton(tabview.tab("Selección Parámetros"),text="Iniciar", command=enviar_seleccion)
sidebar_button_2.grid(row=5, column=3, padx=20, pady=10)

textbox = customtkinter.CTkTextbox(tabview.tab("Selección Parámetros"), width=250)
textbox.grid(row=5, column=0, padx=(20, 0), pady=(20, 0), sticky="nsew")

textbox = customtkinter.CTkTextbox(tabview.tab("Verificación Muestra"), width=250)
textbox.grid(row=0, column=0, padx=(20, 0), pady=(20, 0), sticky="nsew")

titulo_switch = customtkinter.CTkLabel(tabview.tab("Verificación Muestra"), text="Manual Automatico")
titulo_switch.grid(row=0, column=1, columnspan=1, padx=10, pady=10, sticky="")
switch = customtkinter.CTkSwitch(tabview.tab("Verificación Muestra"), text=" ")
switch.grid(row=0, column=3, padx=10, pady=(0, 20))

boton_acep = customtkinter.CTkButton(tabview.tab("Verificación Muestra"),text="Detener")
boton_acep.grid(row=1, column=1, padx=20, pady=10)

boton_rech = customtkinter.CTkButton(tabview.tab("Verificación Muestra"),text="Iniciar")
boton_rech.grid(row=1, column=3, padx=20, pady=10)

textbox = customtkinter.CTkTextbox(tabview.tab("Control de los ejes"), width=200)
textbox.grid(row=0, column=0, padx=(20, 0), pady=(20, 0), sticky="nsew")

# set_home = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="Set Home", command=reset_ejes)
# set_home.grid(row=0, column=1, padx=20, pady=10)
    
# go_home = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="Go to Home")
# go_home.grid(row=0, column=2, padx=20, pady=10)

reset_ejex= tkinter.IntVar(value=0)
reset_ejey= tkinter.IntVar(value=0)
reset_ejez= tkinter.IntVar(value=0)

# ejex = customtkinter.CTkLabel(tabview.tab("Control de los ejes"), text="Eje X")
# ejex.grid(row=2, column=0, columnspan=1, padx=10, pady=10, sticky="")
# slider_x = customtkinter.CTkSlider(tabview.tab("Control de los ejes"),from_=0,to=900, variable=reset_ejex,command=sliderx_event)
# slider_x.grid(row=3, column=0, rowspan=5, padx=(10, 10), pady=(10, 10), sticky="ns")

# ejey = customtkinter.CTkLabel(tabview.tab("Control de los ejes"), text="Eje Y")
# ejey.grid(row=2, column=1, columnspan=1, padx=10, pady=10, sticky="")
# slider_y = customtkinter.CTkSlider(tabview.tab("Control de los ejes"),from_=0,to=50,variable=reset_ejey, command=slidery_event)
# slider_y.grid(row=3, column=1, rowspan=5, padx=(10, 10), pady=(10, 10), sticky="ns")

# ejez = customtkinter.CTkLabel(tabview.tab("Control de los ejes"), text="Eje Z")
# ejez.grid(row=2, column=2, columnspan=1, padx=10, pady=10, sticky="")
# slider_z = customtkinter.CTkSlider(tabview.tab("Control de los ejes"),from_=0,to=100, variable=reset_ejez,command=sliderz_event)
# slider_z.grid(row=3, column=2, rowspan=5, padx=(10, 10), pady=(10, 10), sticky="ns")

y_pos = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="Y Pos",command=incremento_y)
y_pos.grid(row=1, column=2, padx=20, pady=10)
y_neg = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="Y Neg",command=decremento_y)
y_neg.grid(row=3, column=2, padx=20, pady=10)
x_pos = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="X Pos",command=incremento_x)
x_pos.grid(row=2, column=3, padx=20, pady=10)
x_neg = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="X Neg",command=decremento_x)
x_neg.grid(row=2, column=1, padx=20, pady=10)
z_pos = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="Z Pos",command=incremento_z)
z_pos.grid(row=3, column=4, padx=20, pady=10)
z_neg = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="Z Neg",command=decremento_z)
z_neg.grid(row=1, column=4, padx=20, pady=10)


app.mainloop()
arduino.close()