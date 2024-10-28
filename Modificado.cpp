#include <stdio.h>
#include <string.h>

#define CAPACIDAD_RESERVAS 2
#define CAPACIDAD_USUARIOS 2

enum Ext_Cancha_futbol { PEQUENA, MEDIANA, GRANDE };

struct Detalle {
    int tiempo; 
    char horario[10]; 
};

struct Usuarios {
    int DNI;
    char nom[20];
    char apellido[20];
    char usuario[20];
    char contrasena[20];
    char correo [40];
};

struct Cancha {
    enum Ext_Cancha_futbol tam_cancha;
    int cant_personas;
};

struct Reservacion {
    int num_reservacion;
    struct Usuarios usuario;
    struct Detalle det_res;
    struct Cancha cancha;
    float precio;
    enum Ext_Cancha_futbol ext_cancha_futbol;
};

struct Usuarios usuarios[CAPACIDAD_USUARIOS];
struct Reservacion reservas[CAPACIDAD_RESERVAS];
int contadorReservas = 0;
int contadorUsuarios = 0;
float total_recaudado = 0.0;

// Credenciales del administrador
const char adminUser[] = "admin";
const char adminPass[] = "1234";

// Funciones
void agregarPrecio(struct Reservacion *nueva_res) {
    switch (nueva_res->ext_cancha_futbol) {
        case PEQUENA:
            nueva_res->precio = 1000 * nueva_res->det_res.tiempo;
            nueva_res->cancha.cant_personas = 8;
            break;
        case MEDIANA:
            nueva_res->precio = 2000 * nueva_res->det_res.tiempo;
            nueva_res->cancha.cant_personas = 12;
            break;
        case GRANDE:
            nueva_res->precio = 3000 * nueva_res->det_res.tiempo;
            nueva_res->cancha.cant_personas = 16;
            break;
        default:
            printf("\nOpcion incorrecta\n");
            break;
    }
}

int horario_Disponible(const char *horario) {
    for (int i = 0; i < contadorReservas; i++) {
        if (strcmp(reservas[i].det_res.horario, horario) == 0) {
            return 0; // El horario ya esta reservado
        }
    }
    return 1; // El horario esta disponible
}

void agregar_Reservacion() {
    if (contadorReservas < CAPACIDAD_RESERVAS) {
        struct Reservacion *nueva_res = &reservas[contadorReservas];

        printf("\nIngrese el documento (DNI): ");
        scanf("%d", &nueva_res->usuario.DNI);

        printf("\nIngrese su nombre: ");
        scanf("%s", nueva_res->usuario.nom);

        printf("\nIngrese su apellido: ");
        scanf("%s", nueva_res->usuario.apellido);

        int cancha;
        printf("\nIngrese el tipo de cancha (0 Pequena, 1 Mediana, 2 Grande): ");
        scanf("%d", &cancha);
        nueva_res->ext_cancha_futbol = (enum Ext_Cancha_futbol)cancha;

        printf("\nIngrese el tiempo (horas): ");
        scanf("%d", &nueva_res->det_res.tiempo);

         char horario[10];
        int horarioDisponible;
        do {
            printf("\nIngrese el horario (hh:mm): ");
            scanf("%s", horario);

            horarioDisponible = horario_Disponible(horario);
            if (!horarioDisponible) {
                printf("Este horario ya esta reservado. Elija otro.\n");
            }
        } while (!horarioDisponible);

        strcpy(nueva_res->det_res.horario, horario); // Guardar el horario
        agregarPrecio(nueva_res);

        total_recaudado += nueva_res->precio;
        nueva_res->num_reservacion = contadorReservas + 1;
        contadorReservas++;
    } else {
        printf("Capacidad maxima de reservaciones alcanzada.\n");
    }
}

void mostrar_Reservaciones() {
    for (int i = 0; i < contadorReservas; i++) {
        struct Reservacion *res = &reservas[i];

        printf("\nReserva %d:", res->num_reservacion);
        printf("\nDNI: %d", res->usuario.DNI);
        printf("\nNombre: %s", res->usuario.nom);
        printf("\nApellido: %s", res->usuario.apellido);
        printf("\nCorreo: %s", res->usuario.correo);
        printf("\nTipo de Cancha: %s", res->ext_cancha_futbol == PEQUENA ? "Pequena" :
                                         res->ext_cancha_futbol == MEDIANA ? "Mediana" : "Grande");
        printf("\nTiempo (horas): %d", res->det_res.tiempo);
        printf("\nHorario: %s", res->det_res.horario);
        printf("\nCantidad de Personas: %d", res->cancha.cant_personas);
        printf("\nPrecio: %.2f", res->precio);
    }
     if (contadorReservas == 0) {
        printf("\nNo hay reservaciones disponibles.\n");
        return;
    }
}

void mostrar_Total_Recaudado() {
    printf("\nTotal recaudado: %.2f\n", total_recaudado);
}

int iniciar_Sesion_Admin() {
    char usuario[20], contrasena[20];
    printf("\nIngrese usuario admin: ");
    scanf("%s", usuario);
    printf("Ingrese contrasena: ");
    scanf("%s", contrasena);
    
    if (strcmp(usuario, adminUser) == 0 && strcmp(contrasena, adminPass) == 0) {
        printf("\nInicio de sesion exitoso.\n");
        return 1;
    } else {
        printf("\nCredenciales incorrectas.\n");
        return 0;
    }
}

int iniciar_Sesion_Usuario() {
    char usuarioOcorreo[50], contrasena[20];
    int opcion;
    printf("\nIngrese su nombre de usuario o correo: ");
    scanf("%s", usuarioOcorreo);
    printf("Ingrese su contrasena: ");
    scanf("%s", contrasena);
    
    for (int i = 0; i < contadorUsuarios; i++) {
    	if ((strcmp(usuarios[i].usuario, usuarioOcorreo) == 0 || 
             strcmp(usuarios[i].correo, usuarioOcorreo) == 0) && 
            strcmp(usuarios[i].contrasena, contrasena) == 0) {
            printf("\nInicio de sesion exitoso.\n");
            printf("\nDesea realizar una reservacion?: 1(si) 0(no)");
            scanf("%d",&opcion);
            if(opcion == 1){
            	agregar_Reservacion();
						}
						return 1;
        }
    }
    printf("\nCredenciales incorrectas.\n");
    return 0;
}

void registrar_Usuario() {
    if (contadorUsuarios < CAPACIDAD_USUARIOS) {
        struct Usuarios *nuevo_usuario = &usuarios[contadorUsuarios];
        printf("\nIngrese su documento (DNI): ");
        scanf("%d", &nuevo_usuario->DNI);
        printf("Ingrese su nombre: ");
        scanf("%s", nuevo_usuario->nom);
        printf("Ingrese su apellido: ");
        scanf("%s", nuevo_usuario->apellido);
        printf("Ingrese un nombre de usuario: ");
        scanf("%s", nuevo_usuario->usuario);
        printf("Ingrese una contrasena: ");
        scanf("%s", nuevo_usuario->contrasena);
        printf("Ingrese su correo electronico: ");
        scanf("%s", nuevo_usuario->correo);
        
        contadorUsuarios++;
        printf("\nUsuario registrado con exito.\n");
    } else {
        printf("Capacidad maxima de usuarios alcanzada.\n");
    }
}

int main() {
    int opcion;
    int esAdmin = 0; // Variable para verificar si el usuario es administrador

    do {
        printf("\nIngrese la operacion a realizar");
        printf("\n(1) Iniciar sesion como administrador");
        printf("\n(2) Registrarse como nuevo usuario");
        printf("\n(3) Iniciar sesion como usuario");
        printf("\n(0) Para finalizar: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                esAdmin = iniciar_Sesion_Admin();
                break;
            case 2:
                registrar_Usuario();
                break;
            case 3:
                if (iniciar_Sesion_Usuario()) {
                    esAdmin = 0; // Usuario comun
                }
                break;
            case 0:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opcion no valida.\n");
                break;
        }

        if (esAdmin == 1) {
            do {
                printf("\nOperaciones de administrador:");
                printf("\n(1) Registrar usuario");
                printf("\n(2) Agregar reservacion de cancha");
                printf("\n(3) Mostrar reservaciones");
                printf("\n(4) Calcular el total recaudado");
                printf("\n(0) Para finalizar sesion: ");
                scanf("%d", &opcion);

                switch (opcion) {
                    case 1:
                        registrar_Usuario();
                        break;
                    case 2:
                        agregar_Reservacion();
                        break;
                    case 3:
                        mostrar_Reservaciones();
                        break;
                    case 4:
                        mostrar_Total_Recaudado();
                        break;
                    case 0:
                        esAdmin = 0; // Salir de la sesion de administrador
                        break;
                    default:
                        printf("Opcion no valida.\n");
                        break;
                }
            } while (esAdmin);
        }

    } while (opcion != 0);

    return 0;
}

