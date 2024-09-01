from flask import Flask, request, render_template
from datetime import datetime
import os

app = Flask(__name__)

# Dictionary with tag names
tag_names = {
    "5eca5651": "Black",
    "fc2c1fcf": "Green",
    "6e774161": "Red",
    "9cd1e05d": "White",
    "8a1a165": "Blue"
}

# Track the state (logged in/ out) of each tag
tag_states = {}

# Tag name = key | log entries = list of dictionaries
logs_dict = {}

@app.route('/log', methods=['POST'])
def log_data():
    uid = request.form['uid']
    
    # Look up the tag name based on the UID
    tag_name = tag_names.get(uid, "Unknown Tag")
    
    # Get the current time and date
    current_time = datetime.now().strftime("%H:%M:%S %Y-%m-%d")
    
    # Determine the action based on current state
    if tag_states.get(uid) == "in":
        action = "logout"
        tag_states[uid] = "out"
        
        # Update the last entry with logout time
        if tag_name in logs_dict and logs_dict[tag_name]:
            logs_dict[tag_name][-1]["logout"] = current_time
    else:
        action = "login"
        tag_states[uid] = "in"
        
        # Add new login entry for this tag
        if tag_name not in logs_dict:
            logs_dict[tag_name] = []
        logs_dict[tag_name].append({"login": current_time, "logout": None})
    
    # Store the entry in a data.txt file
    with open("data.txt", "a") as file:
        file.write(f"{tag_name} {action} at {current_time}\n")
    
    return f"{tag_name} {action} at {current_time}\n"

# Route for the HTML page
@app.route('/')
def index():
    return render_template('index.html', logs=logs_dict)

@app.route('/logs')
def get_logs():
    # Read the logs and format them
    rows = []
    for tag_name, entries in logs_dict.items():
        for entry in entries:
            login_time = entry["login"]
            logout_time = entry["logout"]
            rows.append(f"<tr><td>{tag_name}</td><td>{login_time}</td><td>{logout_time}</td></tr>")
        
    return ''.join(rows)

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8000)
