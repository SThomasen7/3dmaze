#!/bin/python3

import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d
import re

def get_data(file_name="temp.txt"):

    pos = list()
    lookat = list()

    with open(file_name) as fptr:
        for line in fptr:
            if 'Moving camera new position' in line:
                nums = re.findall(r'[-+]?\d*\.?\d+(?:[eE][-+]?\d+)?', line)
                x, y, z = map(float, nums[-3:])
                pos.append((x, y, z))
            if 'lookat' in line:
                nums = re.findall(r'[-+]?\d*\.?\d+(?:[eE][-+]?\d+)?', line)
                x, y, z = map(float, nums[-3:])
                lookat.append((x, y, z))

    return pos, lookat

def plot(points):

    xs, ys, zs = zip(*points)

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    ax.plot(xs, ys, zs)

    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")

    plt.show()

def plot(points):

    xs, ys, zs = zip(*points)

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    ax.plot(xs, ys, zs)

    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")

    plt.show()


def plot_sep(points):
    xs, ys, zs = zip(*points)
    t = range(len(points))

    fig, axes = plt.subplots(3, 1, sharex=True, figsize=(10, 6))

    axes[0].plot(t, xs)
    axes[0].set_ylabel("X")
    axes[0].grid(True)

    axes[1].plot(t, ys)
    axes[1].set_ylabel("Y")
    axes[1].grid(True)

    axes[2].plot(t, zs)
    axes[2].set_ylabel("Z")
    axes[2].set_xlabel("Frame")
    axes[2].grid(True)

    plt.tight_layout()
    plt.show()
