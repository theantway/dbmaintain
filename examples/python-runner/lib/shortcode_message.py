from __future__ import unicode_literals, print_function, division

from task.database import *
from task.mo import STOP_ALL, STOP_MULTIPLE_SUBSCRIPTION, STOP_NO_SUBSCRIPTION, HELP_MULTIPLE_SUBSCRIPTION, HELP_NO_SUBSCRIPTION, CONFIRM_MULTIPLE, UNKNOWN

def add_shortcode_message(shortcode_id, type, text):
    message_id = insert("INSERT INTO message(id) values(default)")
    add('shortcode_message', id=message_id, shortcode_id=shortcode_id, message_type=type, text=text)

def add_shortcode_messages(shortcode, dedicated):
    shortcode_id = scalar("SELECT id FROM shortcode WHERE shortcode=%(shortcode)s", dict(shortcode=shortcode))

    if dedicated:
        add_shortcode_message(shortcode_id, STOP_ALL, "mobileStorm:This message confirms that you have discontinued this Service for ##ACCOUNT.NAME##. Questions contact 866.492.7886 or help@mstorm.biz")
        add_shortcode_message(shortcode_id, STOP_MULTIPLE_SUBSCRIPTION, "Your Keywords: ##KEYWORDS##. Reply with KEYWORD to stop or STOPALL.")
        add_shortcode_message(shortcode_id, STOP_NO_SUBSCRIPTION, "mobileStorm:This message confirms that you have discontinued this Service for ##ACCOUNT.NAME##. Questions contact 866.492.7886 or help@mstorm.biz")
        add_shortcode_message(shortcode_id, HELP_MULTIPLE_SUBSCRIPTION, "Text 1 of these ##KEYWORDS## + HELP for info. Contact help@mstorm.biz or 866.492.7886")
        add_shortcode_message(shortcode_id, HELP_NO_SUBSCRIPTION, "mobileStorm: Alerts. Msg&data chgs apply. Reply STOP to be removed or STOPALL for all. Contact help@mstorm.biz 4 help or 866.492.7886")
        add_shortcode_message(shortcode_id, CONFIRM_MULTIPLE, "Your keywords: ##KEYWORDS##. Reply with KEYWORD to confirm one list or All to confirm all. Contact help@mstorm.biz or 866.492.7886")
        add_shortcode_message(shortcode_id, UNKNOWN, "Sorry we did not understand your request, please check your message or keyword spelling and reply again or email help@mstorm.biz")
    else:
        add_shortcode_message(shortcode_id, STOP_ALL, "mobileStorm:This message confirms that you have discontinued all services for ##SHORTCODE##. Questions contact 866.492.7886 or help@mstorm.biz")
        add_shortcode_message(shortcode_id, STOP_MULTIPLE_SUBSCRIPTION, "Your Keywords: ##KEYWORDS##. Reply with KEYWORD to stop or STOPALL.")
        add_shortcode_message(shortcode_id, STOP_NO_SUBSCRIPTION, "mobileStorm:This message confirms that you have discontinued this Service. Questions contact 866.492.7886 or help@mstorm.biz")
        add_shortcode_message(shortcode_id, HELP_MULTIPLE_SUBSCRIPTION, "Text 1 of these ##KEYWORDS## + HELP for info. Contact help@mstorm.biz or 866.492.7886")
        add_shortcode_message(shortcode_id, HELP_NO_SUBSCRIPTION, "mobileStorm: Alerts. Msg&data chgs apply. Reply STOP to be removed or STOPALL for all. Contact help@mstorm.biz 4 help or 866.492.7886")
        add_shortcode_message(shortcode_id, CONFIRM_MULTIPLE, "Your keywords: ##KEYWORDS##. Reply with KEYWORD to confirm one list or All to confirm all. Contact help@mstorm.biz or 866.492.7886")
        add_shortcode_message(shortcode_id, UNKNOWN, "Sorry we did not understand your request, please check your message or keyword spelling and reply again or email help@mstorm.biz")
