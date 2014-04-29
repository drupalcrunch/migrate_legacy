<?php
/**
 * @file
 * Migration for source (original language) data.
 */

class LegacySourceBlogMigration extends LegacyBaseMigration {

    public function __construct($arguments) {
        parent::__construct($arguments);

        $this->description = t('Import blog data from legacy database.');

        $query = $this->getConnection()->select('resource', 'r');
        $query->join('resource_translation', 'rt', 'r.resource_id = rt.resource_id');
        $query->fields('r', array('resource_id', 'member_id'));
        $query->fields('rt', array('title', 'body', 'lang_code'));
        $query->condition('r.type', 'blog', '=');
        $query->condition('rt.lang_code', 'en', '=');
        $query->orderBy('r.resource_id', 'ASC');

        $this->source = new MigrateSourceSQL($query);

        $this->destination = new MigrateDestinationNode('blog_post');

        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'resource_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'description' => 'Resource ID',
                    'alias' => 'rt',
                ),
            ),
            MigrateDestinationNode::getKeySchema()
        );

        $this->addFieldMapping('title', 'title');
        $this->addFieldMapping('body', 'body');
        $this->addFieldMapping('body:format')
            ->defaultValue('full_html');

        $this->addFieldMapping('language', 'lang_code');

        // Mapping member_id to node uid
        $this->addFieldMapping('uid', 'member_id')
            ->sourceMigration('LegacyUsers')
            ->defaultValue(1);
    }

}

/**
 * @file
 * Migration for source (original language) data.
 */

class LegacyTranslatedBlogMigration extends LegacyBaseMigration {

    public function __construct($arguments) {
        parent::__construct($arguments);

        $this->description = t('Import blog data from legacy database.');

        $this->dependencies = array('LegacySourceBlogMigration');

        $query = $this->getConnection()->select('resource', 'r');
        $query->join('resource_translation', 'rt', 'r.resource_id = rt.resource_id');
        $query->fields('r', array('resource_id', 'member_id'));
        $query->fields('rt', array('title', 'body', 'lang_code'));
        $query->condition('r.type', 'blog', '=');
        $query->condition('r.type', 'blog', '=');
        $query->condition('rt.lang_code', 'en', '<>');
        $query->orderBy('rt.resource_translation_id', 'ASC');

        $this->source = new MigrateSourceSQL($query);
        $this->destination = new MigrateDestinationNode('blog_post');

        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'resource_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'description' => 'Resource ID',
                    'alias' => 'rt',
                ),
            ),
            MigrateDestinationNode::getKeySchema()
        );


        $this->addFieldMapping('title', 'title');
        $this->addFieldMapping('body', 'body');
        $this->addFieldMapping('body:format')
            ->defaultValue('full_html');

        $this->addFieldMapping('language', 'lang_code');

        $this->addFieldMapping('tnid', 'resource_id')->sourceMigration('LegacySourceBlogMigration');

        // Mapping member_id to node uid
        $this->addFieldMapping('uid', 'member_id')
            ->sourceMigration('LegacyUsers')
            ->defaultValue(1);

        // Default field mappings without source value
        $this->addFieldMapping('status')
            ->defaultValue(1);


    }

    function prepareRow($row) {
        if (is_null($row->title)) {
            $row->title = 'Blog ' . $row->resource_id;
        }
    }

    function prepare(&$node, $row) {
        if (isset($node->tnid) && ($source = node_load($node->tnid))) {
            $node->translation_source = $source;
        }
    }
}