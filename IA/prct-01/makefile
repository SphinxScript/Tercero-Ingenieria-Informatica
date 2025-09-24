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
TARGET  = ejecutable.out

# Archivos fuente
SOURCES = $(wildcard $(SRC_DIR)/*.cc)

# Regla por defecto
all: $(TARGET)

# Compilar y enlazar en un paso
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Ejecutar el programa con un fichero de data
run: $(TARGET)
	./$(TARGET) data/entrada.txt

# Limpiar
clean:
	rm -f $(TARGET)
