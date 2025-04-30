def calculate_l_u_v(X):
    # Iterate through possible values of l, u, and v
    for l in range(256):
        for u in range(16):
            for v in range(16):
                # Calculate the expression and check if it equals X
                if 256*(l-1) + 16*u + v == X:
                    return l, u, v
    
    # If no solution is found
    return None, None, None

# Replace 'your_X_value' with the actual value of X
your_X_value = 12020  # Replace this with the value of X you have

# Calculate l, u, and v
l, u, v = calculate_l_u_v(your_X_value)

if l is not None:
    print(f"l = {l}, u = {u}, v = {v}")
else:
    print("No solution found for the given X value.")
