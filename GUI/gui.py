import json
import tkinter as tk
from tkinter import ttk, messagebox

class JSONTreeViewer(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("JSON Tree Viewer")
        self.geometry("1920x1080")
        self.configure(bg="#2E3440")
        
        # Styling for the tree view
        self.style = ttk.Style(self)
        self.style.configure("Treeview", font=("Helvetica", 12), background="#3B4252", foreground="#ECEFF4", fieldbackground="#3B4252")
        self.style.configure("Treeview.Heading", font=("Helvetica", 14, "bold"), foreground="#88C0D0")
        self.style.layout("Treeview", [('Treeview.treearea', {'sticky': 'nswe'})])
        
        self.tree = ttk.Treeview(self)
        self.tree.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        self.tree.heading("#0", text="JSON Viewer", anchor=tk.W)
        self.tree.tag_configure("key", foreground="#A3BE8C")
        self.tree.tag_configure("value", foreground="#EBCB8B")

        # Load the JSON file immediately when the app starts
        self.load_json_file("../JSON/buyer.json")  # Replace with your hardcoded JSON file path
    
    def load_json_file(self, file_path):
        """Load JSON data from the hardcoded file path."""
        try:
            with open(file_path, "r") as file:
                data = json.load(file)
            self.clear_tree()
            self.insert_json("", data)
        except Exception as e:
            messagebox.showerror("Error", f"Failed to load JSON file: {e}")

    def insert_json(self, parent, data):
        """Insert JSON data into the tree view."""
        if isinstance(data, dict):
            for key, value in data.items():
                node = self.tree.insert(parent, "end", text=str(key), open=True, tags=("key",))
                self.insert_json(node, value)
        elif isinstance(data, list):
            for index, item in enumerate(data):
                node = self.tree.insert(parent, "end", text=f"[{index}]", open=True, tags=("key",))
                self.insert_json(node, item)
        else:
            self.tree.insert(parent, "end", text=str(data), tags=("value",))

    def clear_tree(self):
        """Clear all items from the tree view."""
        for item in self.tree.get_children():
            self.tree.delete(item)

if __name__ == "__main__":
    app = JSONTreeViewer()
    app.mainloop()
