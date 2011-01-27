from __future__ import unicode_literals, print_function, division

import sys
from os.path import dirname, abspath

sys.path.append(dirname(abspath(__file__))+'/../../lib')

from shortcode_message import add_shortcode_messages

if __name__ == "__main__":
    add_shortcode_messages('91355', False)
