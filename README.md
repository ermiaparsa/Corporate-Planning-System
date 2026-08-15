# Corporate Planning

A C++ implementation of the 'Firm Planning' academic model. This repo focuses on resource allocation, production management, and operational decision-making. We are bridging theoretical modeling with AI-driven optimization to build intelligent, modular, and efficient business solutions. Designed for academic research and scalable software architecture.

## Features

- **Resource Allocation Modeling**: Efficiently distribute limited firm resources based on operational constraints.
- **Production Simulation**: Simulate business outcomes based on variable input conditions.
- **Modular Architecture**: Built with modern C++ principles, allowing for easy expansion of logic and data models.
- **Optimization Ready**: Architected to support advanced mathematical algorithms and heuristic approaches.

## Roadmap

- [x] Initial project structure setup
- [ ] Implement core planning models from the reference textbook
- [ ] Develop testing scenarios for production constraints
- [ ] Integrate advanced optimization techniques
- [ ] Explore AI/ML agents for automated decision support

##🛠 Tech Stack
To ensure high performance, mathematical precision, and modularity, the following technologies are utilized:

Language: C++20 (utilizing modern features for memory management and performance).
Build System: CMake (for cross-platform compatibility and efficient dependency management).
Mathematics & Optimization:
Standard Template Library (STL) for robust data structures.
Potential integrations: Eigen or Armadillo (for high-performance linear algebra).
Version Control: Git & GitHub (for professional workflow and collaboration).
Development Environment: VS Code / CLion / Linux (Ubuntu) (optimized for C++ development).
Testing Framework: Google Test or Catch2 (to ensure mathematical model accuracy).

##Architecture
CorporatePlanning/
├── CMakeLists.txt              # Top-level build configuration
├── include/
│   └── corporate_planning/     # Public headers
│       ├── core/               # Core domain models
│       ├── planning/           # Firm planning engine
│       ├── optimization/       # Optimization & heuristic solvers
│       ├── simulation/         # Production & resource simulation
│       ├── data/               # Data access & persistence layer
│       └── api/                # API/CLI interfaces
├── src/                        # Implementation files (.cpp)
├── tests/                      # Unit & integration tests
├── data/                       # Static input data (JSON/CSV)
├── config/                     # Configuration files
└── docs/                       # Additional documentation

##🧩 Design Layers
Layer	Responsibility
Core	Domain entities: Resource, Product, Constraint, Firm, Plan
Planning	Implements the reference textbook model: allocation logic, production decisions, and operational rules
Optimization	Mathematical solvers and heuristics (linear programming wrappers, greedy, local search)
Simulation	Monte Carlo and scenario-based outcome simulation with variable input conditions
Data	Persistence, configuration loading, and dataset parsing
API	Command-line and (future) HTTP interface for external interaction
##⚙️ Design Principles
Separation of Concerns: each module has a single, well-defined responsibility.
Dependency Inversion: core logic depends on abstractions, not concrete data sources.
Testability: pure business logic is isolated from I/O for easy unit testing.
Extensibility: new optimization algorithms can be added without touching the core.

##Installation
###✅ Prerequisites
C++ Compiler: GCC 10+ / Clang 12+ / MSVC 19+
CMake: version 3.16 or higher
Git: for version control
Optional: ninja for faster builds
###📦 Steps

#### 1. Clone the repository
git clone https://github.com/ermiaparsa/CorporatePlanning.git
cd CorporatePlanning

#### 2. Configure the build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

#### 3. Build the project
cmake --build build -j$(nproc)

#### 4. Run the executable
./build/bin/corporate_planning

##Environment Variables
###🔧 Configuration via .env
Create a .env file in the project root (or set these in your shell):

#### Build & Runtime
BUILD_TYPE=Release
LOG_LEVEL=info

#### Data Paths
DATA_DIR=./data
OUTPUT_DIR=./output

#### Solver Configuration
SOLVER_TIMEOUT_MS=5000
OPTIMIZATION_THREADS=4
RANDOM_SEED=42

#### Optional: External optimizer integration
OPT_SOLVER_BACKEND=glpk

##### Future: AI/ML agent endpoint
ML_MODEL_PATH=./models/planner.onnx

##Running Locally
###🚀 Development Workflow

#### Build in debug mode
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)

#### Run the main simulation
./build/bin/corporate_planning --scenario data/scenarios/basic.json

#### Run with verbose logging
LOG_LEVEL=debug ./build/bin/corporate_planning
###🎮 Example: Run a Production Simulation
./build/bin/corporate_planning \
  --mode simulate \
  --input data/scenarios/production_a.json \
  --output output/results.json \
  --seed 42

##API Documentation
📌 The HTTP API is planned as the bridge between the C++ planning core and future AI agents.

###Planned Endpoints

Method	Endpoint	Description
POST	/api/v1/plans	Create a new production plan
GET	/api/v1/plans/{id}	Retrieve plan details
POST	/api/v1/simulate	Run a simulation scenario
GET	/api/v1/resources	List available resources
POST	/api/v1/optimize	Trigger optimization solver
###Example Request
POST /api/v1/simulate
{
  "resources": [
    { "id": "r1", "type": "labor", "capacity": 120 }
  ],
  "products": [
    { "id": "p1", "demand": 300, "unit_cost": 4.5 }
  ],
  "constraints": { "max_hours": 40 },
  "seed": 42
}
###Example Response

{
  "status": "ok",
  "total_production": 275,
  "resource_utilization": 0.86,
  "execution_time_ms": 142
}

##Testing
###🧪 Running the Test Suite

#### Enable tests and build
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build -j$(nproc)

#### Run all tests
ctest --test-dir build --output-on-failure

#### Run a specific test
./build/tests/planning_tests
###📋 Test Coverage Targets
Unit Tests: ResourceAllocation, ConstraintSolver, PlanBuilder
Integration Tests: end-to-end production simulation scenarios
Property Tests: invariants of the planning model (e.g., non-negative allocation)

##Database Schema
📌 Currently the project is file-based (JSON/CSV). The schema below represents the domain model and the planned relational mapping.

###🗄 Domain Entities

CREATE TABLE resources (
    id          TEXT PRIMARY KEY,
    name        TEXT NOT NULL,
    type        TEXT NOT NULL,      -- labor | material | machine | capital
    capacity    REAL NOT NULL,
    unit_cost   REAL
);

CREATE TABLE products (
    id            TEXT PRIMARY KEY,
    name          TEXT NOT NULL,
    demand        REAL,
    unit_price    REAL,
    unit_cost     REAL
);

CREATE TABLE constraints (
    id           TEXT PRIMARY KEY,
    resource_id  TEXT REFERENCES resources(id),
    expr         TEXT NOT NULL,     -- mathematical expression
    bound        REAL NOT NULL
);

CREATE TABLE plans (
    id            TEXT PRIMARY KEY,
    created_at    TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    status        TEXT DEFAULT 'draft',
    total_cost    REAL,
    total_profit  REAL
);

CREATE TABLE plan_items (
    plan_id     TEXT REFERENCES plans(id),
    product_id  TEXT REFERENCES products(id),
    quantity    REAL NOT NULL,
    PRIMARY KEY (plan_id, product_id)
);
###🔗 Entity Relationships

resources 1 ── * constraints
plans    1 ── * plan_items * ── 1 products

##Future Improvements
###🤖 AI & ML Integration
Train an RL (Reinforcement Learning) agent to learn optimal resource-allocation policies.
Replace heuristic solvers with learned models for complex constraint spaces.
Expose the planning core as an inference backend via the planned HTTP API.
###⚡ Optimization Engine
Integrate OR-Tools or HiGHS for mixed-integer linear programming (MILP).
Add genetic algorithms and simulated annealing for NP-hard variants.
Implement parallel evaluation of candidate solutions.
###🧩 Core Enhancements
Full multi-period (dynamic) planning support beyond single-period models.
Uncertainty modeling with robust and stochastic optimization.
Real-time constraint validation and sensitivity analysis.
###🛠 Developer Experience
Add CI/CD pipeline (GitHub Actions) for automated testing and static analysis.
Generate API docs and Doxygen code documentation.
Provide Docker container for reproducible academic experiments.
###📊 Visualization & Reporting
Dashboard output for resource utilization and production efficiency.
LaTeX/TikZ-based report generation for academic publications.

## Contributing

This is an academic research project. Suggestions for improving the planning model, optimization methods, and simulation features are welcome. Please open an issue before submitting a pull request.

## License

This project is licensed under the MIT License.

## Author

**Ermia Parsamanesh**

- GitHub: [@ermiaparsa](https://github.com/ermiaparsa)
