#!/usr/bin/env python3
"""
generate_boom.py - synthesize a punchy explosion sound for KaboomTyper.

Writes Resources/sounds/boom.wav as 16-bit PCM, stereo, 44100 Hz (matching the
game's SDL audio mixer). The boom layers:
  * a low sine sweep (~120 -> 40 Hz) with exponential decay for the body,
  * a fast-attack white-noise burst with exponential decay + simple low-pass
    for the blast,
  * a short bright crackle tail.
Mixed, normalized and soft-clipped for impact.

Run once from anywhere:  python Scripts/generate_boom.py
(no third-party deps; uses only the standard library)
"""

import math
import os
import random
import struct
import wave

SAMPLE_RATE = 44100
DURATION = 0.6  # seconds
N = int(SAMPLE_RATE * DURATION)

random.seed(1234)  # deterministic output


def soft_clip(x):
    # tanh-style soft saturation for a fatter, non-harsh limit
    return math.tanh(1.6 * x)


def main():
    body = [0.0] * N        # low-frequency boom
    blast = [0.0] * N       # noise burst (low-passed)
    crackle = [0.0] * N     # bright snap tail

    # --- low sine sweep body ---
    phase = 0.0
    for i in range(N):
        t = i / SAMPLE_RATE
        # sweep frequency 120 Hz -> 40 Hz over the sound
        freq = 120.0 - 80.0 * (i / N)
        phase += 2.0 * math.pi * freq / SAMPLE_RATE
        env = math.exp(-3.2 * t)            # decays over ~0.3s
        body[i] = math.sin(phase) * env

    # --- noise blast (one-pole low-pass for a thick "whoomph") ---
    lp = 0.0
    alpha = 0.25  # low-pass coefficient (lower = duller)
    for i in range(N):
        t = i / SAMPLE_RATE
        white = random.uniform(-1.0, 1.0)
        lp += alpha * (white - lp)
        # fast attack (~6ms) then exponential decay
        attack = min(1.0, t / 0.006)
        env = attack * math.exp(-7.5 * t)
        blast[i] = lp * env

    # --- bright crackle tail (short, sparse, higher frequency) ---
    for i in range(N):
        t = i / SAMPLE_RATE
        white = random.uniform(-1.0, 1.0)
        env = math.exp(-22.0 * t)
        # sparse pops
        if random.random() < 0.5:
            crackle[i] = white * env

    # --- mix ---
    mixed = [0.0] * N
    for i in range(N):
        s = 1.05 * body[i] + 0.9 * blast[i] + 0.35 * crackle[i]
        mixed[i] = soft_clip(s)

    # normalize to -1..1
    peak = max(1e-9, max(abs(s) for s in mixed))
    gain = 0.97 / peak
    mixed = [s * gain for s in mixed]

    # stereo: tiny per-channel variation for width
    left = mixed
    right = [soft_clip(mixed[i] * 0.98 + 0.02 * (blast[i])) for i in range(N)]

    out_dir = os.path.join(os.path.dirname(__file__), "..", "Resources", "sounds")
    out_dir = os.path.normpath(out_dir)
    out_path = os.path.join(out_dir, "boom.wav")

    with wave.open(out_path, "wb") as w:
        w.setnchannels(2)
        w.setsampwidth(2)        # 16-bit
        w.setframerate(SAMPLE_RATE)
        frames = bytearray()
        for i in range(N):
            l = int(max(-32768, min(32767, left[i] * 32767)))
            r = int(max(-32768, min(32767, right[i] * 32767)))
            frames += struct.pack("<hh", l, r)
        w.writeframes(bytes(frames))

    print("Wrote", out_path, "(%.2fs, %d frames, 16-bit stereo %d Hz)" % (DURATION, N, SAMPLE_RATE))


if __name__ == "__main__":
    main()
