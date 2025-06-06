# ⚡ FLEXFIRE-X: EMG-Controlled Cyberware-Inspired Projectile System

A wearable, servo-powered arm attachment that extends and fires a spring-loaded projectile using EMG muscle signals—built to mimic the immersive cyberware experience of *Cyberpunk 2077*.

![Studio Render](<Blend Files/Renders/Studio/Studio Shot.png>)

## 🚀 Overview

**WRISTLAUNCH-X** is a bio-integrated device that mounts on the user's forearm and responds to intuitive muscle movements:

- ⬇️ **Wrist rotation** (palm down) → **Barrel extends**
- 💪 **Forearm flexion** → **Projectile fires**

This project explores how EMG signals can control physical actuators to feel like a seamless extension of the human body.

![Demo Animation](<Demo Render.gif>)

---

## 🧠 Concept & Inspiration

Inspired by the **Projectile Launch System** from *Cyberpunk 2077*, this prototype brings sci-fi to life with:

- EMG-based gesture control
- A concealed-to-exposed transformation mechanism
- Arduino-powered spring-loaded projectile launch

![Concept Art Reference](<Blend Files/Textures/Reference.jpg>)
**Art by Ben Andrews on [ArtStation](https://www.artstation.com/artwork/WKqEVD)*

---

## 💪 EMG Signal Detection
- Electrodes placed over the *flexor carpi radialis* muscle on the forearm help detect proper movement of the wrist and forearm flexion.
- Muscle signals are filtered via moving average to produce more stable results.
- Three flexion states are used for control:
  - **Low Flexion (<50)** → Barrel stays retracted
  - **Medium Flexion (>50)** → Barrel extends
  - **High Flexion (>500)** → Projectile is launched

---

## 🔧 3D Design

The FLEXFIRE-X uses a total of nine 3D printed components that were designed in Blender and printed in PLA. 

An exploded view of the device shows the layout of each part:

![Exploded View](<Blend Files/Renders/Exploded/Exploded.png>)
**The Mounting Bracket, Rack and Pinion Gear were modified from [potentprintables](https://www.thingiverse.com/thing:3170748) design on Thingiverse*

---

## 🔫 Launching Mechanism

The projectile is launched using a spring-loaded barrel system controlled by a servo-actuated locking mechanism. Here's how it works:

1. The rocket is inserted into the barrel, with its fins aligning with internal grooves to ensure proper orientation and constrain its rotation.
2. As the rocket is pushed in, the spring compresses, storing potential energy.
3. A servo rotates a locking rod, holding the rocket securely in place under spring tension.
4. When the servo rotates back to its original position, the lock is disengaged, allowing the spring to decompress and launch the rocket forward.
5. **Note:** To increase launch force, multiple springs can be nested concentrically within the barrel.

![Launcher Demo](<Blend Files/Renders/Launcher/Launcher Demo.gif>)

---

## ⚙️ Hardware & Components

The following components were used to build the device and circuit

| Component               | Purpose                             |
|------------------------|-------------------------------------|
| Arduino Nano RP2040 Connect       | Main microcontroller                |
| EMG Sensor | Muscle signal detection       |
| SG90 Servo Motor | Projectie launch trigger        |
| Continuous Servo | Barrel extension/retraction    |
| Pen Springs | Launches the projectile        |
| 5V Red Laser | Aim Assistance |
| 9V Batteries | On-body power management |
