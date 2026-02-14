import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

files = {
    "F Prime Events": "EVT_bench_results.csv",
    "F Prime Telemetry": "TLM_bench_results.csv",
    "Raw TCP": "TCP_buffer_bench_results.csv",
    "Raw UDP": "UDP_buffer_bench_results.csv"
}

def generate_analysis(file_map):
    all_data = []
    stats_list = []

    for label, path in file_map.items():
            df = pd.read_csv(path)
            df['label'] = label
            all_data.append(df)
            stats_list.append({
                "Transport": label,
                "Mean (ms)": df['latency_ms'].mean(),
                "Std Dev": df['latency_ms'].std(),
                "P99": df['latency_ms'].quantile(0.99)
            })

    combined_df = pd.concat(all_data)

    print ("STATS TABLE")
    print(pd.DataFrame(stats_list).to_string(index=False))

    ## 2 box plots
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
    sns.set_style("whitegrid")

    fprime_data = combined_df[combined_df['label'].str.contains("F Prime")]
    sns.boxplot(x='label', y='latency_ms', data=fprime_data, ax=ax1, palette="mako")
    ax1.set_title("F Prime Middleware Latency", fontweight='bold')
    ax1.set_ylabel("Latency (ms)")
    ax1.set_xlabel("")

    raw_data = combined_df[~combined_df['label'].str.contains("F Prime")]
    sns.boxplot(x='label', y='latency_ms', data=raw_data, ax=ax2, palette="flare")
    ax2.set_title("Raw Socket Latency", fontweight='bold')
    ax2.set_ylabel("Latency (ms)")
    ax2.set_xlabel("")

    plt.suptitle("Latency Analysis: F Prime Middleware vs. Raw Sockets", fontsize=16)
    plt.tight_layout(rect=[0, 0.03, 1, 0.95])

    plt.savefig('latency_boxplot.png', dpi=300)

if __name__ == "__main__":
    generate_analysis(files)