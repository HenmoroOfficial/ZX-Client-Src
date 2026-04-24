#ifndef __CPPGEN_GNET_SKILL239
#define __CPPGEN_GNET_SKILL239
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill239:public Skill
    {
      public:
        enum
        { SKILL_ID = 239 };
          Skill239 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill239Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            bool Quit (Skill * skill) const
            {
                return skill->GetPlayer ()->GetHp () < 120 + 120 * skill->GetLevel ();
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                int hpnow = skill->GetPlayer ()->GetHp ();
                int hpdec = 120 + 120 * skill->GetLevel ();
                  skill->GetPlayer ()->SetHp (1 + norm (skill->GetPlayer ()->GetHp () / hpdec) * (hpnow - hpdec));
                  skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill239Stub ():SkillStub (239)
        {
            occupation = 8;
            name = L"炼气还神";
            nativename = "炼气还神";
            icon = "炼气还神.dds";
            maxlevel = 12;
            maxlearn = 8;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 2;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 589;
            talent[1] = 592;
            talent[2] = 596;
            talent_size = 3;
            action[0] = "青云门_炼气还神";
            action[1] = "青云门_炼气还神";
            action[2] = "青云门_炼气还神";
            action[3] = "青云门_炼气还神";
            action[4] = "青云门_炼气还神";
            action[5] = "青云门_炼气还神";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_炼气还神";
            action[16] = "青云门_骑乘_炼气还神";
            action[17] = "青云门_骑乘_炼气还神";
            action[18] = "青云门_骑乘_炼气还神";
            action[19] = "青云门_骑乘_炼气还神";
            action[20] = "青云门_骑乘_炼气还神";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "技能攻击1";
            rangetype = 5;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill239Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 230 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 10000 * skill->GetT2 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 50, 52, 54, 56, 58, 60, 62, 64 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               120 + 120 * skill->GetLevel (),
                               2 + 2 * skill->GetLevel (), INT ((1.0 + 0.05 * skill->GetLevel ()) * (120 + 120 * skill->GetLevel ())));

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (2000 + 2000 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->
                SetValue (((1.0 + 0.05 * skill->GetLevel ()) * (120 +
                                                                120 * skill->GetLevel ()) * norm (skill->GetPlayer ()->GetHp () / (120.5 +
                                                                                                                                   120 *
                                                                                                                                   skill->
                                                                                                                                   GetLevel ())) +
                           (0.95 + 0.05 * skill->GetLevel ()) * skill->GetPlayer ()->GetHp () * norm ((120.5 + 120 * skill->GetLevel ()) /
                                                                                                      skill->GetPlayer ()->GetHp ())) * (1 +
                                                                                                                                         0.6 *
                                                                                                                                         skill->
                                                                                                                                         GetT0 ()) *
                          (1 + INT (zrand (100) / (100 - 16 * skill->GetT0 ()))));
            skill->GetVictim ()->SetMpgen (1);
            skill->GetVictim ()->SetTime (6050 * norm (skill->GetT1 ()));
            skill->GetVictim ()->SetRatio (0.03 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncskilldamage (1);
            return true;
        }
#endif
    };
}
#endif
