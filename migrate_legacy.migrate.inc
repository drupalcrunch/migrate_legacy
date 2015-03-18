<?php

/**
 * Impelements hook_migrate_api().
 */
function migrate_legacy_migrate_api() {
    return array(
        'api' => 2,
        'groups' => array(
            'ifes' => array(
                'title' => t('IFES'),
            ),
        ),
        'migrations' => array(
            'LegacyUsers' => array(
                'class_name' => 'LegacyUserMigration',
                'group_name' => 'ifes',
            ),
            'LegacySourceBlogs' => array(
                'class_name' => 'LegacySourceBlogMigration',
                'group_name' => 'ifes',
                'dependencies' => array(
                    'LegacyUsers',
                ),
            ),
            'LegacyTranslatedBlogs' => array(
                'class_name' => 'LegacyTranslatedBlogMigration',
                'group_name' => 'ifes',
                'dependencies' => array(
                    'LegacyUsers',
                ),
            ),
            'LegacyEvents' => array(
                'class_name' => 'LegacyEventsMigration',
                'group_name' => 'ifes',
                'dependencies' => array(
                    'LegacyUsers',
                ),
            ),
        ),
    );
}
