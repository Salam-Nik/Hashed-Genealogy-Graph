# Hashed-Genealogy-Graph

Hashed-Genealogy-Graph is a data structure designed for storing family trees with an emphasis on high-speed access, secure data storage, and preservation of personal privacy.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Secure Hash Usage](#secure-hash-usage)
- [Trie Dictionary Structure](#trie-dictionary-structure)
  - [Trie Dictionary Operations](#trie-dictionary-operations)
  - [Trie Operations](#trie-operations)
  - [Time Complexity](#time-complexity)
- [Functionality](#functionality)
- [Getting Started](#getting-started)
  - [Installation](#installation)
  - [Usage](#usage)
    - [Python Library Integration](#1-python-library-integration)
    - [C++ Library Integration](#2-c-library-integration)
       - [Web Application Integration](#web-application-integration)
          - [Web Application Features](#web-application-features)
          - [Web Application Architecture](#web-application-architecture)
          - [Getting Started with the Web Application](#getting-started-with-the-web-application)

## Introduction

Hashed-Genealogy-Graph is a powerful and secure data structure tailored for storing and managing family trees. With a focus on high-speed access, privacy preservation, and scalability, this genealogy system aims to provide comprehensive functionality for exploring and understanding familial relationships.

## Features

- **Fast Access:** Utilizes a Trie structure for efficient and quick access to family tree elements, ensuring optimal performance even with extensive datasets.
- **Privacy Protection:** Implements the SHA-256 algorithm for secure hashing, safeguarding personal data from unauthorized access. This ensures the confidentiality and integrity of stored information, in compliance with [NIST FIPS PUB 180-4](http://csrc.nist.gov/publications/fips/fips180-4/fips-180-4.pdf).
- **Scalability:** Engineered to handle large volumes of genealogical data, accommodating expansive family trees while maintaining efficiency.

## Secure Hash Usage

In the Trie Dictionary Structure for Hashed-Genealogy-Graph, a secure hash function plays a crucial role in generating unique identifiers (hash values) for individuals in the family tree. The choice of a secure hash algorithm is essential to ensure data integrity, confidentiality, and resistance to various cryptographic attacks.

### Why SHA-256?

SHA-256, a member of the SHA-2 family of hash functions, was chosen for several reasons:

1. **Security:** SHA-256 is considered a secure and robust hash function. It produces a 256-bit (32-byte) hash value, making it computationally infeasible to find two different inputs that produce the same hash output (collision resistance).

2. **Standardization:** SHA-256 is a widely accepted and standardized hash algorithm, as defined by the National Institute of Standards and Technology (NIST) in [FIPS PUB 180-4](http://csrc.nist.gov/publications/fips/fips180-4/fips-180-4.pdf). Its prevalence and standardization make it a reliable choice for cryptographic applications.

3. **Performance:** While SHA-256 provides a high level of security, it also maintains reasonable performance for the intended use case of generating unique identifiers for family tree elements.

### Algorithm Overview

The SHA-256 algorithm follows a series of steps to transform an input message into a fixed-size hash value:

1. **Padding:** The input message is padded to a length that is a multiple of the block size.
2. **Parsing:** The padded message is divided into blocks, and each block undergoes processing.
3. **Message Schedule:** A message schedule is generated from the block, and a series of transformations are applied.
4. **Compression Function:** The hash value is updated through a series of bitwise operations, logical functions, and additions.
5. **Final Hash:** The final hash value is derived by concatenating the updated hash components.

### Time Complexity

The time complexity of SHA-256 is generally considered to be linear with respect to the size of the input message. The algorithm processes the input in blocks, and the number of blocks influences the overall computation time. In practice, SHA-256 provides a balance between security and performance, making it a suitable choice for generating secure and unique identifiers in the context of the Hashed-Genealogy-Graph.

Feel confident in the use of SHA-256 within this Trie Dictionary Structure, as it aligns with established cryptographic standards and provides a robust foundation for secure hash operations.

## Trie Dictionary Structure

The Trie dictionary serves as a fundamental component of the Hashed-Genealogy-Graph, offering efficient storage and retrieval of family tree elements. Below is a comprehensive overview of Trie Dictionary Operations tailored for this specific application:

### Trie Dictionary Operations

#### Insertion

- **Description:** When a new person is added to the family tree, their unique identifier (hash value) is inserted into the Trie along with their associated information. This involves creating nodes in the Trie for each character in the hash value.

#### Retrieval

- **Description:** To find information about a specific person in the family tree, the Trie allows quick retrieval using their unique identifier (hash value). It efficiently navigates the Trie structure character by character to locate and return the associated data.

#### Search

- **Description:** The Trie can determine whether a specific hash value (person) exists in the family tree. The `find` operation checks if the Trie contains the given identifier, helping identify if a person is part of the family.

#### Key Enumeration

- **Description:** The Trie supports listing all persons and their associated information in the family tree. The `values` operation traverses the Trie, collecting pairs of hash values and corresponding data into a list for easy enumeration.

### Trie Operations

1. **Insertion (`insert`):**
   - Insert a key-value pair into the Trie.

2. **Search (`contains` and `find`):**
   - `contains`: Check whether a given key exists in the Trie.
   - `find`: Retrieve the node corresponding to a given key in the Trie.

3. **Access (`operator[]`):**
   - Access the value associated with a particular key using the subscript operator (`[]`).

4. **Deletion (`~Trie` and `deleteSubtree`):**
   - Destructor (`~Trie`): Clean up the memory allocated for the Trie.
   - `deleteSubtree`: Delete a subtree rooted at a specified node.

5. **Iteration (`begin` and `end`):**
   - `begin`: Obtain an iterator pointing to the first element in the Trie.
   - `end`: Obtain an iterator pointing past the last element in the Trie.

6. **Utility Functions (`getKey` and `values`):**
   - `getKey`: Retrieve the key associated with a given value in the Trie.
   - `values`: Get a vector of all key-value pairs in the Trie.

These Trie Dictionary Operations play a crucial role in achieving fast access and optimal performance within Hashed-Genealogy-Graph. They provide an efficient way to manage and retrieve information about individuals in the family tree.

## Functionality

### isAncestor

**Description:**
- Determines if a person with `id1` is an ancestor of the person with `id2`.
- Utilizes the SHA-256 hash values for efficient retrieval from the adjacency list.
  
**Time Complexity:**
- O(N), where N is the number of ancestors in the family tree.

### isSibling

**Description:**
- Checks if two persons, identified by `id1` and `id2`, share at least one parent.
- Utilizes the SHA-256 hash values for efficient retrieval from the adjacency list.
  
**Time Complexity:**
- O(N), where N is the number of parents shared between the two persons.

### isDistantRelative

**Description:**
- Determines if two persons, identified by `id1` and `id2`, are distant relatives.
- Checks if they share a common ancestor using the `isAncestor` function and finding the common ancestor.
  
**Time Complexity:**
- O(N), where N is the number of ancestors in the family tree.

### findCommonAncestor

**Description:**
- Finds the common ancestor of two persons identified by `id1` and `id2`.
- Utilizes the `findAllAncestorsBFS` function to find all ancestors using Breadth-First Search.
  
**Time Complexity:**
- O(N), where N is the number of ancestors in the family tree.

### findFurthestDescendant

**Description:**
- Finds the furthest descendant of a person identified by `id`.
- Utilizes a recursive approach to traverse the family tree and find the maximum distance to a descendant.
  
**Time Complexity:**
- O(N), where N is the number of descendants in the family tree.

### findMostDistantRelationship

**Description:**
- Finds the most distant relationship pair in the family tree using the diameter of the graph.
- Utilizes BFS to calculate the shortest paths between all pairs of persons.
  
**Time Complexity:**
- O(N^2), where N is the number of individuals in the family tree.

## Getting Started

Explore the diverse functionalities of Hashed-Genealogy-Graph by following the installation and usage instructions outlined below.

### Installation

```bash
sudo apt install python3-venv
python3 -m venv env
source env/bin/activate
pip3 install cython
chmod +x build.sh
./build.sh
```

## Usage

### 1. Python Library Integration

#### Using the Python Library

The Trie Dictionary Structure can be accessed conveniently using a Python library. Although the core implementation is in C++, we provide a Python library named "HashedGenealogyGraph.py" to enable seamless integration with Python applications.

#### Prerequisites

Before using the Python library, ensure you have the necessary dependencies installed:

```bash
sudo apt install python3-venv
python3 -m venv env
source env/bin/activate
pip3 install cython
chmod +x pyBuild.sh
./pyBuild.sh
```

#### Usage

1. Activate the virtual environment:
   ```bash
   source env/bin/activate
   ```

2. Place the "HashedGenealogyGraph.py" library in your Python project path.

3. Utilize the library in your Python code as follows:
   ```python
   import HashedGenealogyGraph

   # Your code here...
   ```

### 2. C++ Library Integration

#### Using the C++ Library

The Trie Dictionary Structure can be seamlessly integrated as a library in your C++ projects. Ensure you have the necessary prerequisites installed:

```bash
#!/bin/bash

for file in src/*.cpp; do
  g++ -c "$file" -o "build/$(basename "$file" .cpp).o"
done
```

Include the Trie library in your C++ files as follows:

```cpp
#include "GenealogyGraph.h"

// Your C++ code here...
```

Feel free to explore and leverage the Trie Dictionary Structure in your projects, be it Python or C++, for efficient storage and retrieval of family tree elements.

## Additional Web Application Integration

In addition to the core functionalities provided by Hashed-Genealogy-Graph, I've integrated a web application using CrowCpp to enhance user interaction and visualization of family trees.

### Web Application Features

- **CrowCpp Integration:** The web application is built using Crow, a C++ framework for creating HTTP or WebSocket web services. This integration enables the creation of a dynamic and responsive user interface for exploring family trees.

- **Interactive Visualization:** Leveraging JavaScript and the Balkan library, the web application provides an interactive and visually appealing representation of family trees. Users can navigate through generations, view detailed information about individuals, and explore relationships easily.

- **Real-time Updates:** The web application ensures real-time updates to reflect any changes made to the family tree. Whether it's adding new members or modifying existing relationships, the web interface keeps the displayed information synchronized with the underlying Hashed-Genealogy-Graph data structure.

### Web Application Architecture

The CrowCpp backend serves as the communication bridge between the Hashed-Genealogy-Graph data structure and the user interface. It handles requests for family tree information, updates, and interactions.

The front-end, implemented in JavaScript with the Balkan library, communicates with the CrowCpp backend through AJAX requests. This separation of concerns allows for a modular and maintainable architecture, enabling easy future enhancements to the user interface.

### Getting Started with the Web Application

To run the web application:

1. Ensure the Hashed-Genealogy-Graph library is properly installed as per the provided instructions.

2. Build the CrowCpp backend:

    ```bash
    chmod +x build.sh
    ./build.sh
    ```

3. Run the CrowCpp web server:

    ```bash
    ./main.exe
    ```

4. Open your web browser and navigate to `http://localhost:8080` to access the Hashed-Genealogy-Graph web application.

Explore the combined power of Hashed-Genealogy-Graph and the web application to efficiently manage, visualize, and interact with family trees. Contributions and feedback on the web application integration are highly encouraged. Happy exploring!

![image 1](https://github.com/Salam-Nik/Hashed-Genealogy-Graph/blob/main/images/image1.png)

![image 2](https://github.com/Salam-Nik/Hashed-Genealogy-Graph/blob/main/images/image2.png)

![image 3](https://github.com/Salam-Nik/Hashed-Genealogy-Graph/blob/main/images/image3.png)
## Contributing

Contributions are welcome! If you have any suggestions, improvements, or bug fixes, feel free to open an issue or create a pull request.

## License

This project is licensed under the [MIT License](LICENSE).
```

Feel free to use or modify this according to your needs.
