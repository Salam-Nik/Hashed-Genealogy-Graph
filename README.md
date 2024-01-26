
# Hashed-Genealogy-Graph

Hashed-Genealogy-Graph is a data structure designed for storing family trees with an emphasis on high-speed access, secure data storage, and preservation of personal privacy.

## Target

The primary goals of Hashed-Genealogy-Graph are:

1. **High Speed:** Enable fast access to family tree information.
2. **Privacy Preservation:** Ensure that the stored data remains confidential and is not accessible by outsiders.
3. **Scalability:** The ability to expand and efficiently handle a large amount of data.

## Purpose

Hashed-Genealogy-Graph serves as a genealogy system designed to address issues related to kinship and family relationships. The system provides the following capabilities:

### A) Checking Parent and Child Relationship

The `isAncestor` function checks if a given person is the ancestor of another person. It utilizes the SHA256 algorithm for secure hashing and then employs a recursive algorithm to determine the ancestor relationship.

### B) Checking Sibling Relationship

The `isSibling` function checks if two individuals are siblings. It compares the parents of both persons to find a common parent, indicating a sibling relationship.

### C) Checking Distant Family Relationship

The `isDistantRelative` function determines if two persons are distant relatives, not direct ancestors or siblings. It uses the `isAncestor` function and the `findCommonAncestor` function to identify if a common ancestor exists.

### T) Finding Common Ancestor

The `findCommonAncestor` function takes two person IDs, converts them to hash values, and uses a Breadth-First Search (BFS) approach to find the common ancestor. It returns the hash value of their common ancestor.

### D) Finding Farthest Descendant

The `findFurthestDescendant` function calculates the furthest descendant of a given person by recursively exploring the descendants in each family. It returns the maximum distance among the descendants.

### E) Finding the Most Distant Relatives

Functionality to find the most distant relatives in the family tree is achieved through a combination of ancestor checking and finding common ancestors.

## Privacy and Security Measures

A) **Privacy Protection:**
   - Utilizes the SHA-256 algorithm for hashing, ensuring data privacy and integrity.
   - Implementation based on [NIST FIPS PUB 180-4](http://csrc.nist.gov/publications/fips/fips180-4/fips-180-4.pdf).

B) **Fast Access:**
   - Trie structure is employed for efficient and quick access to family tree elements.
   - A dictionary based on Trie is created for optimized data retrieval.

C) **Scalability:**
   - The system is designed to handle a large volume of data efficiently.

Feel free to contribute, report issues, or suggest improvements to make Hashed-Genealogy-Graph even more robust and versatile.

## License

This project has been released under MIT License.

```bash
sudo apt install python3-venv
python3 -m venv env
source env/bin/activate
pip3 install cython
chmod +x build.sh
./build.sh
```