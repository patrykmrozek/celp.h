#define CELP_TEST
#include "../celp.h"

static celp_links_t links;
static celp_err_t err = CELP_ERR_OK;

typedef struct v3 {
    float x, y, z;
    celp_link_t link;
} v3;

static v3 v;

CELP_TEST_SETUP(links)
{
    celp_links_init(&links);
}

CELP_TESTCASE(links_add)
{
    err = CELP_ERR_OK;
    celp_usize old_count = links.count;
    celp_links_add(&links, &v.link);
    CELP_EXPECT_EQ(links.count, old_count + 1);
}

CELP_TESTCASE(links_unlink)
{
    celp_usize old_count = links.count;
    celp_links_unlink(&links, &v.link);
    CELP_EXPECT_EQ(links.count, old_count -1);
}

CELP_TESTCASE(links_foreach)
{
    v3 va, vb, vc, vd, ve, vf;
    v3 vs[6] = {va, vb, vc, vd, ve, vf};

    for (int i = 0; i < 6; i++) {
        celp_links_add(&links, &vs[i].link);
    }

    int link_count = 0;
    celp_links_foreach(&links, _link) {
        CELP_EXPECT_EQ(&vs[link_count],
                       celp_link_part_of(_link, v3, link));
        link_count++;
    }
}

CELP_TEST_SUITE_START(links)
{
    CELP_TEST_SUITE_ADD_SETUP(links, links);
    CELP_TEST_SUITE_ADD_TEST(links, links_add);
    CELP_TEST_SUITE_ADD_TEST(links, links_unlink);
}
CELP_TEST_SUITE_END(links)

void test_celp_links(void)
{
    CELP_TEST_SUITE_RUN(links);
    CELP_TEST_SUITE_REPORT(links);
    CELP_TEST_SUITE_DESTROY(links);
}
