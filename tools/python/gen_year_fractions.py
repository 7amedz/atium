import json
import sys

import QuantLib as ql

CONVENTIONS = {
    "act_365f": ql.Actual365Fixed(),
    "act_360": ql.Actual360(),
    "thirty_360_bb": ql.Thirty360(ql.Thirty360.BondBasis),
    "thirty_360_e": ql.Thirty360(ql.Thirty360.European),
}

PAIRS = [
    # --- ordinary input -----------------------------------------------------
    ("2026-09-07", "2026-12-07", "ordinary quarter"),
    ("2026-01-01", "2027-01-01", "exactly one year; ACT/360 still gives 1.0139"),
    ("2027-09-07", "2028-09-07", "366 days, contains a leap day"),
    ("2026-08-31", "2026-09-30", "month end to month end"),
    ("2026-01-31", "2026-03-31", "31st to 31st"),
    ("2026-01-31", "2026-02-28", "into February"),
    ("2026-11-30", "2027-02-28", "crosses a year boundary"),
    # --- edge cases ---------------------------------------------------------
    ("2026-06-15", "2026-06-15", "same day; must be 0, not an error"),
    ("2026-01-30", "2026-01-31", "distinct dates worth zero under 30/360"),
    ("2026-02-28", "2026-03-31", "33 fictional days in a 30-day month; BB != 30E"),
    ("2028-02-29", "2028-08-31", "leap-day start, tau > half a year; BB != 30E"),
]


def main() -> None:
    cases = []
    for iso1, iso2, note in PAIRS:
        d1 = ql.DateParser.parseISO(iso1)
        d2 = ql.DateParser.parseISO(iso2)
        cases.append(
            {
                "d1": iso1,
                "d2": iso2,
                "note": note,
                "actual_days": d2 - d1,
                "year_fraction": {
                    name: dc.yearFraction(d1, d2) for name, dc in CONVENTIONS.items()
                },
            }
        )

    json.dump(
        {
            "source": f"QuantLib {ql.__version__}",
            "tolerance": {"kind": "absolute", "value": 1e-12},
            "cases": cases,
        },
        sys.stdout,
        indent=2,
    )
    sys.stdout.write("\n")


if __name__ == "__main__":
    main()
