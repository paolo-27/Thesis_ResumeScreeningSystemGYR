import random
import pandas as pd
import matplotlib.pyplot as plt

#Generate dataset
workers = [f"Worker_{i}" for i in range(1, 11)]
days = [f"Day_{d}" for d in range(1, 8)]

data = []
for worker in workers:
    for day in days:
        hours_worked = random.randint(4, 10)
        hours_distracted = random.randint(0, 5)
        tasks_completed = max(0, hours_worked - hours_distracted + random.randint(-2, 2))
        stress_level = random.randint(3, 10)
        data.append([worker, day, hours_worked, hours_distracted, tasks_completed, stress_level])

df = pd.DataFrame(data, columns=["Worker", "Day", "Hours_Worked", "Hours_Distracted", "Tasks_Completed", "Stress_Level"])

#Save dataset to Excel
df.to_excel("Remote_Work_Productivity.xlsx", index=False)

#Chart 1 - Average tasks per worker
avg_tasks = df.groupby("Worker")["Tasks_Completed"].mean()

plt.figure(figsize=(10,6))
avg_tasks.plot(kind="bar", color="skyblue")
plt.title("Average Tasks Completed per Worker")
plt.xlabel("Worker")
plt.ylabel("Tasks Completed")
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig("Chart1_AvgTasksPerWorker.png")
plt.show()

#Chart 2 - Stress trend of one worker
worker_choice = "Worker_1"
df_worker = df[df["Worker"] == worker_choice]

plt.figure(figsize=(8,5))
plt.plot(df_worker["Day"], df_worker["Stress_Level"], marker="o", linestyle="-", color="red")
plt.title(f"Stress Level Trend of {worker_choice}")
plt.xlabel("Day")
plt.ylabel("Stress Level")
plt.grid(True, linestyle="--", alpha=0.6)
plt.tight_layout()
plt.savefig("Chart2_StressTrend_Worker1.png")
plt.show()

#Chart 3 - Bar: Avg tasks by distraction hours
avg_by_distraction = df.groupby("Hours_Distracted")["Tasks_Completed"].mean()

plt.figure(figsize=(8,5))
avg_by_distraction.plot(kind="bar", color="orange")
plt.title("Average Tasks Completed by Hours Distracted")
plt.xlabel("Hours Distracted")
plt.ylabel("Average Tasks Completed")
plt.xticks(rotation=0)
plt.tight_layout()
plt.savefig("Chart3_TasksByDistraction.png")
plt.show()


