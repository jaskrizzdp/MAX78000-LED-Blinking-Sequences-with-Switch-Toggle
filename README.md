# MAX78000 LED Blinking Sequences with Switch Toggle

**A MAX78000 microcontroller project demonstrating two LED animation sequences controlled with a single switch.**

---

## Project Overview

This project implements LED animations on the **MAX78000 microcontroller** using **eight LEDs** and **one switch**. The system allows toggling between two LED sequences in real-time:

- **Sequence 1**: Shrinks the LED pattern inward  
- **Sequence 2**: Expands the LED pattern outward  

The switch enables users to switch between the two sequences during operation, showcasing real-time control and microcontroller-based LED animation.  

---

## LED Sequence Patterns

**Sequence 1 (Inward Shrink):** 

11111111<br>
01111110<br>
00111100<br>
00011000




**Sequence 2 (Outward Expand):**  

00011000<br>
00111100<br>
01111110<br>
11111111





These ASCII representations show the LED states over time, where `1` is ON and `0` is OFF.

---

## Key Features

- Two LED animation sequences: inward shrink and outward expand  
- Real-time sequence toggle using a single switch  
- Demonstrates MAX78000 GPIO and timing control  
- Simple interactive LED project for learning embedded programming  

---

## How It Works

1. **LED Control**: LEDs are connected to GPIO pins on the MAX78000.  
2. **Animation Logic**: Sequence patterns are coded to create inward or outward animations.  
3. **Switch Input**: The single switch toggles between the two sequences in real-time.  
4. **Loop Execution**: The microcontroller continuously runs the animation logic, checking switch input to switch sequences.  



