#!/usr/bin/env python
from subprocess import Popen, PIPE
import unittest
import argparse
import os
import re

class MetaTest(unittest.TestCase):
    """
    This is a meta-class used as the basis for dynamically
    creating test-classes based on .nls files.
    """

    def goodTest(self):
        """
        This function is dynamically added to the test-class 
        as  aliases such as: test_arith etc.
        It looks up parameters for launcher, program, and good file in self.
        """

        # Use the naming convention to extact paths
        dirs = [txt.lower() for txt in re.findall('[A-Z][^A-Z]*', self.id())][:-1]
        directory = os.sep.join(dirs)
        test_name = self.id().split("test_")[-1].lower()

        program_path = os.sep.join([directory, test_name])+ ".nls"
        good_path = os.sep.join([directory, test_name])+ ".good"

        self.cmd = ["niels", program_path]      # niels command
        self.good = open(good_path).read()      # Suck in the .good file

        p = Popen(self.cmd, stdout=PIPE)        # Run the niels program
        out, err = p.communicate()              # Grab output
        self.assertEqual(out, self.good)        # Compare to content of .good

def cls_factory(cname, tests=[]):
    """Construct a test-class for the unittest to pick up."""

    class Foo(MetaTest):
        pass
    Foo.__name__ = cname

    return Foo

def produce_classes(root):
    """
    Look in root for .nls files and construct test classes for them.
    """
    
    #
    # Dynamically construct unittest.TestCase classes
    #
    classes = {}
    for root, dirs, files in os.walk(root):  # Find tests
        tests = []

        for filename in (fn for fn in files if fn.endswith(".nls")):
            bname, ext = os.path.splitext(os.path.basename(filename))
            test = "test_%s" % bname
            tests.append(test)

        if tests:                           # Create a class for the directory
            cname = "%sTest" % "".join([
                txt.capitalize() for txt in root.split(os.sep)[1:]
            ])
            cls = cls_factory(cname, tests)
            for test in tests:              # Create a test-method for each .nls file
                setattr(cls, test, cls.goodTest)
            classes[cname] = cls
    return classes

if __name__ == '__main__':

    root = os.path.dirname(os.path.abspath(__file__))
    os.chdir(root)

    classes = produce_classes(".")  # Construct test-classes
    for cname in classes:           # Expose them in local scope
        cls = classes[cname]
        locals()[cname] = cls
        del cls                     # Avoid dual definition

    unittest.main()                 # Run class tests
