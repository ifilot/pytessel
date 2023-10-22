import pytessel
import unittest

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
