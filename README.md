1. **Buffer:**  

   Contains code from the lab for string formatting when sending requests.

2. **Connection:**  

   Contains code from the lab for establishing a connection with the server and enabling communication.

3. **Parson:**  

   This is a library for JSON in C, used when you need to send fields in requests.

4. **Request:**  

   Similar to the lab code, but tailored for specific requirements:

   - For POST requests, it sends the URL on one line, the host on the second, the cookie and token (if available) on the next two lines, followed by the payload specific to the task.

   - For DELETE requests, it includes only the access path, host, token, and cookie.

   - For GET requests, it includes the same as DELETE, but uses the GET method.

5. **Helper:**  

   Contains functions that will be called in my client called `program`:

   - Three extraction functions to retrieve the cookie, token, and fields of the books received from the server. I use `strstr` to locate each, and for fields, it should start with `{` or `[` and end with `}` or `]`.

   - The `create_json_user_object` function uses the Parson library to generate a JSON object for processing the username and password that will be read for registration and login. Reading is done carefully to avoid overlapping strings or other issues, with a clear buffer after each read. The reading stops at the newline, not after a space; an error is displayed if a space is encountered, and `getchar` is used to remove any remaining characters in the input buffer.

   - The `register_user` function first checks if the user is invalid and displays an error message if so. If everything is okay, it performs a `post_request` as suggested in the API requirements. Then it checks the returned code; if the return code is 201, it means creation was successful; if not, the user already exists.

   - The `login` function sends a `post_request` and receives a cookie to ensure the user is logged in. I have a flag to verify permissions for `enter_library` and other commands. If everything is fine, the flag is set to 1, and the cookie is obtained from the server.

   - The `access_library` function first checks if the user is logged in; if not, it throws an error. If logged in, it retrieves the token from the server, ensuring access to the library and allowing any library-dependent actions. It also sets the access flag to 1 to confirm privileged access.

   - The `get_books` function retrieves the IDs and books registered so far from the server.

   - The `get_book_details` function is similar to the previous one but retrieves all characteristics of a specific book based on its ID. Input validity is also checked to avoid surprises.

   - The `add_book_helper` function checks the validity of inputs and, if valid, adds them to our JSON object to be sent to the server. String reading is explained above.

   - The `add_book` function checks if we can send the request; if we do, it also verifies the return code.

   - The `delete_book_from_library` function operates similarly to the previous functions, sending a `delete_request` and checking the code, just like the `logout` function, which uses a `get_request`.

6. **Program.c:**  

   In `program.c`, we establish the connection, set the described variables, and call the explained functions.
