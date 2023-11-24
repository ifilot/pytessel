import pytessel
import unittest
import sys, os

# add a reference to load the pytessel library
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))

class TestVersion(unittest.TestCase):

    def test_version(self):
        """
        Test whether verion string exists
        """
        try:
            pytessel.__version__
        except NameError as e:
            raise e
        else:
            print('All in order')

if __name__ == '__main__':
    unittest.main()
