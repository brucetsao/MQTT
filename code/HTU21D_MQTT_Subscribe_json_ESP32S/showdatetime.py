from datetime import datetime
currentDateAndTime = datetime.now()

print("The current date and time is", currentDateAndTime)
# Output: The current date and time is 2022-03-19 10:05:39.482383
currentTime = currentDateAndTime.strftime("%H:%M:%S")
print("The current time is", currentTime)