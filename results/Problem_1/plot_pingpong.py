import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from sklearn.linear_model import LinearRegression
import glob
import re

# Helper function to extract integer size from filename
def extract_n_items(filename):
    match = re.search(r'time_(\d+)M_(same|diff)\.csv', filename)
    if match:
        size_in_m = int(match.group(1))
        return size_in_m * 1_000_000
    return None

# Load and combine all CSVs
def load_data(pattern):
    data = []
    for file in sorted(glob.glob(pattern)):
        df = pd.read_csv(file)
        n = extract_n_items(file)
        if n:
            time = df["avg_one_way_time_seconds"].iloc[0]
            data.append((n, time))
    return pd.DataFrame(data, columns=["n_items", "avg_one_way_time_seconds"])

# Load same and diff data
same_df = load_data("time_*M_same.csv")
diff_df = load_data("time_*M_diff.csv")

# Prepare bytes for linear fit
X_same = same_df["n_items"].values.reshape(-1, 1) * 4  # 4 bytes per int
y_same = same_df["avg_one_way_time_seconds"].values

X_diff = diff_df["n_items"].values.reshape(-1, 1) * 4
y_diff = diff_df["avg_one_way_time_seconds"].values

# Fit linear models
model_same = LinearRegression().fit(X_same, y_same)
model_diff = LinearRegression().fit(X_diff, y_diff)

# Predict for plotting
X_plot = np.linspace(X_same.min(), X_same.max(), 100).reshape(-1, 1)
y_same_pred = model_same.predict(X_plot)
y_diff_pred = model_diff.predict(X_plot)

# Plot
plt.figure(figsize=(8, 6))
plt.plot(X_same / 1e6, y_same, 'o', label='Same Node (data)', markerfacecolor='none')
plt.plot(X_plot / 1e6, y_same_pred, '-', label='Same Node (fit)')

plt.plot(X_diff / 1e6, y_diff, 's', label='Diff Node (data)', markerfacecolor='none')
plt.plot(X_plot / 1e6, y_diff_pred, '--', label='Diff Node (fit)')

plt.xlabel("Data Size (MB)")
plt.ylabel("Average One-Way Time (seconds)")
plt.title("Ping-Pong MPI Communication Time")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("pingpong_communication_plot.png")
plt.show()

# Print latency and bandwidth
def interpret(model, label):
    latency = model.intercept_
    bandwidth = 1 / model.coef_[0] / 1e6  # MB/s
    print(f"{label}:\n  Latency ≈ {latency:.6e} s\n  Bandwidth ≈ {bandwidth:.2f} MB/s\n")

interpret(model_same, "Same Node")
interpret(model_diff, "Diff Node")
