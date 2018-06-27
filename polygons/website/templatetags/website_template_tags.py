from django import template

register = template.Library()

@register.filter
def makelist(upperBound):
    return list(range(upperBound))
