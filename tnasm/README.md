# 🧩 TeenyAT Assembler (`tnasm`)

**`tnasm`** translates `.asm` source files into binary files executable by the **TeenyAT Virtual Machine**.

---

## 🌈 Colorful Hello World
This program makes use of the **[TeenyAT Color LCD](../lcd/README.md)** to draw colors to a screen

```asm
.const LIVESCREEN 0x9000            ; Create constant macro

!main                               ; Create label called main
    str [LIVESCREEN + rA], rA       ; Draw color to screen
    inc rA                          ; Add 1 to rA
    mod rA, 4096                    ; Keep rA in domain [0,4095]
    jmp !main                       ; Jump to the the label !main
```
Save this file as **draw_colors.asm** then run: **`tnasm draw_colors.asm`**  

This will create a binary with the same base file name and generate the following listing:

```asm
        1: [           ]  .const LIVESCREEN 0x9000            ; Create constant macro
        3: [           ]  !main                               ; Create label called main
0x0000  4: [ 11b0 9000 ]      str [LIVESCREEN + rA], rA       ; Draw color to screen
0x0002  5: [ 4d81      ]      inc rA                          ; Add 1 to rA
0x0003  6: [ 6980 1000 ]      mod rA, 4096                    ; Keep rA in domain [0,4095]
0x0005  7: [ a800 0000 ]      jmp !main                       ; Jump to the the label !main
```

Now run the executable with **`lcd draw_colors.bin`** to see the screen fill up!

---

## ⚙️ Assembly Language Basics

> 💡 See the full [Instruction Set Reference](../docs/README.md) for more details on actual instructions

### Register Names:
-  `rA`, `rB`, `rC`, `rD`, `rE`, `rZ` (zero register), `SP` (stack pointer), and `PC` (program counter)
-   The names are case insensitive

---

### Comments:
-   Used to prevent text following it from being assembled
-   Denoted with a **`;`**

---

### Constants: 
- Create a symbolic name for a fixed value
- Denoted as `.constant` or `.const`
- **tnasm** replaces every occurrence of the symbol with its value during assembly

```asm
.const     MY_CONST   0xDEAD
.const     CHAR_CONST 'H'
.constant  ITS_NINE   9
```

Below is the listing of the assembled source code

```asm
1: [           ]  .const     MY_CONST   0xDEAD
2: [           ]  .const     CHAR_CONST 'H'
3: [           ]  .constant  ITS_NINE   9
```
> ⚠️ **Note:** constants do not take up a place in the final binary

---

### Variables:
- Assign a symbolic name to a specific memory location
- Denoted as `.variable` or `.var`
- May be initialized with a value or left uninitialized

```asm
.var      MY_VAR        ; uninitialized variable
.var      CHAR_VAR 'H'
.variable ITS_TEN  10
```

Below is the listing of the assembled source code

```asm
0x0000  1: [ 0000      ]  .var      MY_VAR        ; uninitialized variable
0x0001  2: [ 0048      ]  .var      CHAR_VAR 'H'
0x0002  3: [ 000a      ]  .variable ITS_TEN  10

------------------------------------
WARNING, Line 1: A variable at address 0x0000 is unsafe.  It will be executed as code.
```
> 🚨  **ALERT:** If you place variables at the start of your program, the TeenyAT will try to execute them as code since it starts running from address 0x0000. Add a jump at the beginning to skip over your data and avoid this warning!

---

### Labels: 
- Represent the address of the next piece of data or code under them
- Denoted with an **`!`**

```asm
!main
    set rZ, 0x9000
!hello
    set rZ, !main
```
> ⚠️ **Note:** setting register zero does nothing as it always contains zero :)

Below is the listing of the assembled source code

```asm
        1: [           ]  !main
0x0000  2: [ 0000 9000 ]      set rZ, 0x9000
        3: [           ]  !hello
0x0002  4: [ 0400      ]      set rZ, !main
```
The value of `!main` and `!hello` are **0** and **2** respectfully

---

### Raw Lines: 
- Used to place symbols strings and identifiers continuously into memory
- Denoted with **`.raw`**

```asm
!My_Array
    .raw 10 0xDEAD 0b00_11_00 !hello
!hello
```

Below is the listing of the assembled source code

```asm
        1: [           ]  !My_Array
0x0000  2: [ 000a dead ]  
         : [ 000c 0004 ]      .raw 10 0xDEAD 0b00_11_00 !hello
        3: [           ]  !hello
```
> ⚠️ **Note:** You can now load values from the `!My_Array` label with `lod rA, [!My_Array + rB]` treating **rB** as an index into the array

---

### Strings: 
- Two different ways to denote
  - `""` — stores one 16-bit word per character.
  - `''` — stores two 8-bit characters per 16-bit word.
- Cannot be used with **constants** and **variables**

```asm
!Hello_World_str
    .raw "Hello World"

!Packed_Hello_World_str
    .raw 'Hello World'
```

Below is the listing of the assembled source code

```asm
        1: [           ]  !Hello_World_str
0x0000  2: [ 0048 0065 ]  
         : [ 006c 006c ]  
         : [ 006f 0020 ]  
         : [ 0057 006f ]  
         : [ 0072 006c ]  
         : [ 0064 0000 ]      .raw "Hello World"
        3: [           ]  !Packed_Hello_World_str
0x000c  4: [ 6548 6c6c ]  
         : [ 206f 6f57 ]  
         : [ 6c72 0064 ]      .raw 'Hello World'
```
> ⚠️ **Note:** Now you can treat your string like an array!

---

## 💡 Example: Random Port Output

A simple loop that generates random numbers and writes them to `PORT_A`.

```asm
; --- Internal device constants ---
.const PORT_A_DIR 0x8000
.const PORT_A     0x8002
.const RAND       0x8010

    str [PORT_A_DIR], rZ   ; Set all PORT_A pins as outputs (0)

!main
    lod rA, [RAND]         ; Load random positive value
    str [PORT_A], rA       ; Output random value to PORT_A
    jmp !main              ; Loop forever
```
Save this file as **random_port.asm** then run: `tnasm random_port.asm`

You can load the resulting *`random_port.bin`* in the **[led](../README.md)** example system on the main page

---

## 🎨 Example: Interrupt Color Drawing
This program makes use of **external interrupts** to draw random pixels to the **[TeenyAT Color LCD](../lcd/README.md)** 
```asm
; --- Internal device constants ---
.const RAND                      0x8010
.const INTERRUPT_VECTOR_TABLE    0x8E00
.const INTERRUPT_ENABLE_REGISTER 0x8E10
.const CONTROL_STATUS_REGISTER   0x8EFF

; --- LCD system constants ---
.const UPDATESCREEN 0xA000
.const UPDATE       0xE000

; --- Set up interrupt vector (key press → interrupt 8) ---
    set rA, !key_pressed
    set rB, 8
    str [INTERRUPT_VECTOR_TABLE + rB], rA

; --- Enable interrupt 8 ---
    set rA, 0b00000001_00000000
    str [INTERRUPT_ENABLE_REGISTER], rA

; --- Enable interrupts globally ---
    set rA, 0b0000000000000001
    str [CONTROL_STATUS_REGISTER], rA

!main
    str [UPDATE], rZ      ; Trigger screen refresh
    jmp !main             ; Idle loop

; --- Interrupt Handler: Draw random pixel ---
!key_pressed
    psh rA
    psh rB

    lod rA, [RAND]        ; Get random color
    set rB, rA
    mod rA, 4096          ; Limit to [0, 4095]
    str [UPDATESCREEN + rA], rB

    pop rB
    pop rA
    rti                   ; Return from interrupt
```
Save this file as **key_presses.asm** then run: `tnasm key_presses.asm`

Then execute in the **[LCD system](../lcd/README.md)** : `lcd key_presses.bin`

---