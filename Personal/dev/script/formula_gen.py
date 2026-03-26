import random

operators = ["+", "-", r"\times", r"\div"]
functions = [r"\sin", r"\cos", r"\tan", r"\log", r"\exp"]
greek_letters = [
    r"\alpha",
    r"\beta",
    r"\gamma",
    r"\delta",
    r"\theta",
    r"\lambda",
    r"\mu",
    r"\sigma",
]
integrals = [
    r"\int_{a}^{b} f(x) \,dx",
    r"\oint_{C} \mathbf{F} \cdot d\mathbf{r}",
    r"\sum_{n=1}^{\infty} \frac{1}{n^2}",
]
parentheses = ["({})", "[{}]"]
numbers = [str(random.randint(1, 999)) for _ in range(30)]
latex_wrappers = ["$", "$$", r"\(", r"\)"]

sets = [
    r"X = \{x \in \mathbb{R} \mid x > 0\}",
    r"A = \{n \in \mathbb{N} \mid n \text{ is even}\}",
    r"B = \{x \in \mathbb{Z} \mid x^2 < 10\}",
    r"C = \{x \in \mathbb{R} \mid \sin(x) = 0\}",
    r"D = \{x \in \mathbb{R}^2 \mid \|x\| < 1\}",
]

sequences = [
    r"x_0, x_1, \dots, x_n",
    r"a_1, a_2, \dots, a_n",
    r"\lambda_1, \lambda_2, \dots, \lambda_k",
    r"f_0(x), f_1(x), \dots, f_n(x)",
    r"\phi_1, \phi_2, \dots, \phi_m",
]


def random_formula():
    parts = [
        rf"x^{{{random.randint(2, 5)}}} {random.choice(operators)} {random.choice(greek_letters)}",
        rf"{random.choice(functions)}{random.choice(parentheses).format(random.choice(numbers))}",
        rf"\frac{{{random.choice(numbers)}}}{{{random.choice(numbers)}}}",
        rf"\sqrt{{{random.choice(numbers)}}}",
        rf"\frac{{d}}{{dx}} {random.choice(functions)}{random.choice(parentheses).format('x')}",
        rf"{random.choice(integrals)}",
        rf"{random.choice(numbers)} {random.choice(operators)} {random.choice(numbers)}",
        rf"{random.choice(parentheses).format(random.choice(numbers) + random.choice(operators) + random.choice(numbers))}",
        rf"\left({random.choice(numbers)} {random.choice(operators)} {random.choice(greek_letters)}\right)",
        rf"\left[{random.choice(numbers)} {random.choice(operators)} {random.choice(functions)}({random.choice(numbers)})\right]",
        rf"{random.choice(sets)}",
        rf"{random.choice(sequences)}",
    ]

    formula = " ".join(random.sample(parts, 4))
    if random.random() < 0.5:
        formula = (
            f"{random.choice(latex_wrappers)} {formula} {random.choice(latex_wrappers)}"
        )

    return formula


for _ in range(10):
    print(random_formula())
