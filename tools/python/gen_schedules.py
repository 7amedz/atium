import json
import sys

import QuantLib as ql

CALENDAR = ql.NullCalendar()
CONVENTION = ql.Unadjusted
RULE = ql.DateGeneration.Backward
END_OF_MONTH = False

CASES = [
    # --- regular; nothing capped, every date shares a day of month ------------
    ("2026-09-07", "2029-09-07", 12, "regular annual, nothing capped"),
    ("2026-09-07", "2031-09-07", 6, "regular semi-annual, five years"),
    ("2026-09-07", "2027-09-07", 3, "regular quarterly, one year"),
    ("2026-09-07", "2036-09-07", 12, "ten years, ten periods"),
    # --- capping; the roll day does not exist in every month ------------------
    ("2026-08-31", "2028-08-31", 3, "roll day 31: four caps, two recoveries to 31"),
    ("2026-01-31", "2026-07-31", 3, "31st through February"),
    ("2026-11-30", "2027-02-28", 3, "anchor is a 28-day month end"),
    ("2028-02-29", "2029-02-28", 12, "leap-day start"),
    # --- capping artefact; walk lands before the start, in the start's month ---
    (
        "2026-08-31",
        "2027-02-28",
        3,
        "artefact: walk hits 2026-08-28, short first period",
    ),
    (
        "2026-03-31",
        "2026-09-30",
        3,
        "artefact: walk hits 2026-03-30, short first period",
    ),
    # --- degenerate; tenor below one period, two dates only -------------------
    ("2026-09-07", "2026-10-07", 12, "one month under an annual frequency"),
    ("2026-09-07", "2026-12-07", 12, "one quarter under an annual frequency"),
    ("2026-09-07", "2027-03-07", 12, "six months under an annual frequency"),
    ("2026-09-07", "2027-09-07", 12, "exactly one period"),
]


def main() -> None:
    cases = []
    for start, termination, months, note in CASES:
        sd = ql.DateParser.parseISO(start)
        ed = ql.DateParser.parseISO(termination)
        schedule = ql.Schedule(
            sd,
            ed,
            ql.Period(months, ql.Months),
            CALENDAR,
            CONVENTION,
            CONVENTION,
            RULE,
            END_OF_MONTH,
        )
        dates = [d.ISO() for d in schedule]
        cases.append(
            {
                "start": start,
                "termination": termination,
                "frequency_months": months,
                "note": note,
                "expected_dates": dates,
                "period_count": len(dates) - 1,
            }
        )

    json.dump(
        {
            "source": f"QuantLib {ql.__version__}",
            # dates are exact; there is nothing to compare with a tolerance
            "calendar": "NullCalendar",
            "convention": "Unadjusted",
            "rule": "Backward",
            "end_of_month": END_OF_MONTH,
            "cases": cases,
        },
        sys.stdout,
        indent=2,
    )
    sys.stdout.write("\n")


if __name__ == "__main__":
    main()
