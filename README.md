# Data Structures Final Project

                              Amna Zubair_ 23i-2556
                              Zara Tanveer_23i-2556
                               Ali Asjad_23i-1234

# The Smart Traffic Management System Simulator

The Smart Traffic Management System Simulator is a practical project designed to replicate
real-world traffic management in a city. It optimizes urban traffic flow using data structures like
graphs, heaps, and priority queues. The system focuses on real-time vehicle routing, traffic signal
control, congestion management, and emergency handling without incorporating predictive
features

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Project Mechanics](#game-mechanics)
- [Installation](#installation)
- [Running the code](#running-the-game)
- [Screenshots](#screenshots)
- [Dependencies](#dependencies)
- [Configuration](#configuration)
- [Troubleshooting](#troubleshooting)

## Introduction

In modern urban environments, efficient traffic flow is essential to minimize delays, enhance safety, and prioritize emergency response. This program is a comprehensive traffic simulation system designed to model real-world city traffic scenarios. It integrates advanced routing algorithms, dynamic traffic signals, congestion monitoring, and emergency vehicle prioritization to emulate and manage urban traffic.

The system enables:

- Dynamic routing for both emergency and regular vehicles.
- Real-time management of road conditions, including closures and congestion.
- Effective handling of traffic signal states to streamline vehicle movement.
- Priority-based dispatch and movement of vehicles to maintain order and reduce delays.

By incorporating algorithms like Dijkstra’s and A\*, the system ensures optimal routing while adapting to real-time conditions. This tool provides a robust foundation for understanding and managing complex urban traffic systems.

## Features

The provided code is designed to simulate and manage city traffic flow, incorporating both emergency and regular vehicles. The system models a city as a graph where nodes represent intersections, and edges represent roads with specific travel times. The simulation integrates various real-world traffic management features, including:

- Traffic Signals:

  - Signals at intersections with dynamic green and red light durations.
  - Signals influence regular vehicles but allow priority bypass for emergency vehicles.

- Vehicle Movement:

  - Regular vehicles follow shortest paths using Dijkstra’s algorithm and respect traffic signals.
  - Emergency vehicles prioritize speed and use A\* search algorithm to bypass congestion and signals.

- Congestion Management:

  - Roads are monitored for vehicle counts to detect congestion.
  - Congested paths trigger rerouting to reduce traffic density.

- Road Closures:

  - Roads can be blocked dynamically due to accidents or construction.
  - Vehicles reroute based on updated traffic data.

- Priority Queue:

  - Vehicles are processed based on their priority (emergency vehicles are prioritized).
  - A max-heap ensures the correct handling of vehicle dispatch.

- Simulation Dashboard:

  - Allows real-time traffic simulation with options to visualize:
    - City road network.
    - Current traffic signal status.
    - Congestion levels.
    - Blocked roads.

## Installation

1. Ensure you have `g++` installed on your system.

   For Ubuntu/Debian:

   ```bash
   sudo apt-get install g++
   ```

## Running the Game

2. Compile the code using the provided command:

   ```bash
   g++ q.cpp && a
   ```
