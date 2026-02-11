# Microrécif - Marine Ecosystem Simulation

## Description
This project is a simulation of a micro-reef ecosystem implemented in C++ with a graphical interface using GTKmm. The program simulaties interactions between three types of life forms:
- **Algae**: Primary producers that appear randomly.
- **Corals**: Immobile organisms that grow and reproduce.
- **Scavengers**: Mobile organisms that feed on dead corals.

The simulation allows real-time visualization of these interactions, execution control (start/stop/step), and configuration file management.

## Authors
- Tahri Malik
- Laachfoubi Kamil

## Prerequisites
- C++17 compatible compiler (e.g., g++)
- GUI Library: `gtkmm-4.0`

## Compilation
Use the included `Makefile` to compile the project:

```bash
make
```

To clean object files and the executable:

```bash
make clean
```

## Usage

Launch the simulation by providing a configuration file:

```bash
./projet samples/sim_normal.txt
```

### Interface Controls
- **Start/Stop**: Start or pause the simulation.
- **Step**: Advance the simulation by a single time step.
- **Open**: Load a new configuration file.
- **Save**: Save the current state of the simulation.
- **Exit**: Quit the application.
- **Algae Birth**: Checkbox to enable/disable the random appearance of new algae.

## Project Structure
- `src/`: Source files (.cc)
- `include/`: Header files (.h)
- `samples/`: Simulation example files
- `build/`: Object files generated during compilation

## Simulation Files
The simulation files describe the initial state of the system. The following samples are available:

1. `samples/sim_1_algae.txt`: Simulation with a single algae.
2. `samples/sim_1_scavenger.txt`: Simulation with a single scavenger.
3. `samples/sim_1_coral.txt`: Simulation with a single coral.
4. `samples/sim_normal.txt`: A balanced simulation scenario.
5. `samples/sim_crazy.txt`: A stress test with many entities (collision-free).
