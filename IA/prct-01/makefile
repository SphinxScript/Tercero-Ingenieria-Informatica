# CXX = g++
# CXXFLAGS = -std=c++17 -Wall
# TARGET = ejecutable.out
# SOURCE = alfabeto.cc cadena.cc lenguaje.cc funciones.cc main.cc

# $(TARGET): $(SOURCE)
# 	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# clean:
# 	rm -f $(TARGET)

CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRC_DIR = src
TARGET  = busqueda.out

# Archivos fuente  (todo lo terminado en .cc en el directorio src)
SOURCES = $(wildcard $(SRC_DIR)/*.cc)

# Regla por defecto
all: $(TARGET)

# Compilar y enlazar en un paso
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Limpiar
clean:
	rm -f $(TARGET)
