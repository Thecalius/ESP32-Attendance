
from flask import Flask, request
from datetime import datetime

app = Flask(__name__)

# Dictionary with tag names
tag_names = {
    "5eca5651": "Black",
    "fc2c1fcf": "Green",
    "6e774161": "Red",
    "9cd1e05d": "White",
    "8a1a165": "Blue"
}

@app.route('/log', methods = ['POST'])
def log_data():
    uid = request.form['uid']
    
    # Look up the tag name based on the UID
    tag_name = tag_names.get(uid, "Unknown Tag")
    
    #Get the current time and date
    current_time = datetime.now().strftime("%H:%M:%S %Y-%m-%d")
    
    # Store the tag name along with the current time and date
    with open("data.txt", "a") as file:
        file.write(f"{tag_name} - {current_time}\n")
    if (tag_name == "Unknow Tag"):
        return "Unknown Tag"
    else: 
        return f"{tag_name} logged at {current_time}\n"    

if __name__ == "__main__":
    app.run(host = '0.0.0.0', port = 8000)